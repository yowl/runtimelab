// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// ===========================================================================
// File: perfmap.cpp
//

#include "common.h"

#if defined(FEATURE_PERFMAP) && !defined(DACCESS_COMPILE)
#include <clrconfignocache.h>
#include "perfmap.h"
#include "perfinfo.h"
#include "pal.h"


// The code addresses are actually native image offsets during crossgen. Print
// them as 32-bit numbers for consistent output when cross-targeting and to
// make the output more compact.

#define FMT_CODE_ADDR "%p"

Volatile<bool> PerfMap::s_enabled = false;
PerfMap * PerfMap::s_Current = nullptr;
bool PerfMap::s_ShowOptimizationTiers = false;
unsigned PerfMap::s_StubsMapped = 0;

enum 
{
    DISABLED,
    ALL,
    JITDUMP,
    PERFMAP
};

// Initialize the map for the process - called from EEStartupHelper.
void PerfMap::Initialize()
{
    LIMITED_METHOD_CONTRACT;

    // Only enable the map if requested.
    if (CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapEnabled) == ALL || CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapEnabled) == PERFMAP)
    {
        // Get the current process id.
        int currentPid = GetCurrentProcessId();

        // Create the map.
        s_Current = new PerfMap(currentPid);

        int signalNum = (int) CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapIgnoreSignal);

        if (signalNum > 0)
        {
            PAL_IgnoreProfileSignal(signalNum);
        }

        if (CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapShowOptimizationTiers) != 0)
        {
            s_ShowOptimizationTiers = true;
        }

        s_enabled = true;
    }

    if (CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapEnabled) == ALL || CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapEnabled) == JITDUMP)
    {   
        const char* jitdumpPath;
        char jitdumpPathBuffer[4096];

        CLRConfigNoCache value = CLRConfigNoCache::Get("PerfMapJitDumpPath");
        if (value.IsSet())
        {
            jitdumpPath = value.AsString();
        }
        else
        {
            GetTempPathA(sizeof(jitdumpPathBuffer) - 1, jitdumpPathBuffer);
            jitdumpPath = jitdumpPathBuffer;
        }

        PAL_PerfJitDump_Start(jitdumpPath);

        if (CLRConfig::GetConfigValue(CLRConfig::EXTERNAL_PerfMapShowOptimizationTiers) != 0)
        {
            s_ShowOptimizationTiers = true;
        }
        
        s_enabled = true;
    }
}

// Destroy the map for the process - called from EEShutdownHelper.
void PerfMap::Destroy()
{
    LIMITED_METHOD_CONTRACT;

    if (s_enabled)
    {
        s_enabled = false;
        // PAL_PerfJitDump_Finish is lock protected and can safely be called multiple times
        PAL_PerfJitDump_Finish();
    }
}

// Construct a new map for the process.
PerfMap::PerfMap(int pid)
{
    LIMITED_METHOD_CONTRACT;

    // Initialize with no failures.
    m_ErrorEncountered = false;

    // Build the path to the map file on disk.
    WCHAR tempPath[MAX_LONGPATH+1];
    if(!GetTempPathW(MAX_LONGPATH, tempPath))
    {
        return;
    }

    SString path;
    path.Append(tempPath);
    path.AppendPrintf("perf-%d.map", pid);

    // Open the map file for writing.
    OpenFile(path);

    m_PerfInfo = new PerfInfo(pid);
}

// Construct a new map without a specified file name.
// Used for offline creation of NGEN map files.
PerfMap::PerfMap()
  : m_FileStream(nullptr)
  , m_PerfInfo(nullptr)
{
    LIMITED_METHOD_CONTRACT;

    // Initialize with no failures.
    m_ErrorEncountered = false;

    s_StubsMapped = 0;
}

// Clean-up resources.
PerfMap::~PerfMap()
{
    LIMITED_METHOD_CONTRACT;

    delete m_FileStream;
    m_FileStream = nullptr;

    delete m_PerfInfo;
    m_PerfInfo = nullptr;
}

// Open the specified destination map file.
void PerfMap::OpenFile(SString& path)
{
    STANDARD_VM_CONTRACT;

    // Open the file stream.
    m_FileStream = new (nothrow) CFileStream();
    if(m_FileStream != nullptr)
    {
        HRESULT hr = m_FileStream->OpenForWrite(path.GetUnicode());
        if(FAILED(hr))
        {
            delete m_FileStream;
            m_FileStream = nullptr;
        }
    }
}

// Write a line to the map file.
void PerfMap::WriteLine(SString& line)
{
    STANDARD_VM_CONTRACT;

    if (m_FileStream == nullptr || m_ErrorEncountered)
    {
        return;
    }

    EX_TRY
    {
        // Write the line.
        // The PAL already takes a lock when writing, so we don't need to do so here.
        const char * strLine = line.GetUTF8();
        ULONG inCount = line.GetCount();
        ULONG outCount;
        m_FileStream->Write(strLine, inCount, &outCount);

        if (inCount != outCount)
        {
            // This will cause us to stop writing to the file.
            // The file will still remain open until shutdown so that we don't have to take a lock at this level when we touch the file stream.
            m_ErrorEncountered = true;
        }

    }
    EX_CATCH{} EX_END_CATCH(SwallowAllExceptions);
}

void PerfMap::LogImageLoad(PEAssembly * pPEAssembly)
{
    if (s_enabled && s_Current != nullptr)
    {
        s_Current->LogImage(pPEAssembly);
    }
}

// Log an image load to the map.
void PerfMap::LogImage(PEAssembly * pPEAssembly)
{
    CONTRACTL{
        THROWS;
        GC_NOTRIGGER;
        MODE_PREEMPTIVE;
        PRECONDITION(pPEAssembly != nullptr);
    } CONTRACTL_END;


    if (m_FileStream == nullptr || m_ErrorEncountered)
    {
        // A failure occurred, do not log.
        return;
    }

    EX_TRY
    {
        CHAR szSignature[GUID_STR_BUFFER_LEN];
        GetNativeImageSignature(pPEAssembly, szSignature, ARRAY_SIZE(szSignature));

        m_PerfInfo->LogImage(pPEAssembly, szSignature);
    }
    EX_CATCH{} EX_END_CATCH(SwallowAllExceptions);
}


// Log a method to the map.
void PerfMap::LogJITCompiledMethod(MethodDesc * pMethod, PCODE pCode, size_t codeSize, PrepareCodeConfig *pConfig)
{
    LIMITED_METHOD_CONTRACT;

    CONTRACTL{
        THROWS;
        GC_NOTRIGGER;
        MODE_PREEMPTIVE;
        PRECONDITION(pMethod != nullptr);
        PRECONDITION(pCode != nullptr);
        PRECONDITION(codeSize > 0);
    } CONTRACTL_END;

    if (!s_enabled)
    {
        return;
    }

    const char *optimizationTier = nullptr;
    if (s_ShowOptimizationTiers)
    {
        optimizationTier = PrepareCodeConfig::GetJitOptimizationTierStr(pConfig, pMethod);
    }

    // Logging failures should not cause any exceptions to flow upstream.
    EX_TRY
    {
        // Get the full method signature.
        SString name;
        pMethod->GetFullMethodInfo(name);

        // Build the map file line.
        if (optimizationTier != nullptr && s_ShowOptimizationTiers)
        {
            name.AppendPrintf("[%s]", optimizationTier);
        }
        SString line;
        line.Printf(FMT_CODE_ADDR " %x %s\n", pCode, codeSize, name.GetUTF8());

        // Write the line.
        if(s_Current != nullptr)
        {
            s_Current->WriteLine(line);
        }
        PAL_PerfJitDump_LogMethod((void*)pCode, codeSize, name.GetUTF8(), nullptr, nullptr);
    }
    EX_CATCH{} EX_END_CATCH(SwallowAllExceptions);

}

// Log a pre-compiled method to the perfmap.
void PerfMap::LogPreCompiledMethod(MethodDesc * pMethod, PCODE pCode)
{
    LIMITED_METHOD_CONTRACT;

    if (!s_enabled)
    {
        return;
    }

    // Get information about the NGEN'd method code.
    EECodeInfo codeInfo(pCode);
    _ASSERTE(codeInfo.IsValid());

    IJitManager::MethodRegionInfo methodRegionInfo;
    codeInfo.GetMethodRegionInfo(&methodRegionInfo);

    // Logging failures should not cause any exceptions to flow upstream.
    EX_TRY
    {
        // Get the full method signature.
        SString name;
        pMethod->GetFullMethodInfo(name);

        if (s_ShowOptimizationTiers)
        {
            name.Append(W("[PreJIT]"));
        }

        // NGEN can split code between hot and cold sections which are separate in memory.
        // Emit an entry for each section if it is used.
        if (methodRegionInfo.hotSize > 0)
        {
            PAL_PerfJitDump_LogMethod((void*)methodRegionInfo.hotStartAddress, methodRegionInfo.hotSize, name.GetUTF8(), nullptr, nullptr);
        }

        if (methodRegionInfo.coldSize > 0)
        {
            if (s_ShowOptimizationTiers)
            {
                pMethod->GetFullMethodInfo(name);
                name.Append(W("[PreJit-cold]"));
            }
            PAL_PerfJitDump_LogMethod((void*)methodRegionInfo.coldStartAddress, methodRegionInfo.coldSize, name.GetUTF8(), nullptr, nullptr);
        }
    }
    EX_CATCH{} EX_END_CATCH(SwallowAllExceptions);
}

// Log a set of stub to the map.
void PerfMap::LogStubs(const char* stubType, const char* stubOwner, PCODE pCode, size_t codeSize)
{
    LIMITED_METHOD_CONTRACT;

    if (!s_enabled)
    {
        return;
    }

    // Logging failures should not cause any exceptions to flow upstream.
    EX_TRY
    {
        if(!stubOwner)
        {
            stubOwner = "?";
        }
        if(!stubType)
        {
            stubType = "?";
        }

        SString name;
        // Build the map file line.
        name.Printf("stub<%d> %s<%s>", ++(s_StubsMapped), stubType, stubOwner);
        SString line;
        line.Printf(FMT_CODE_ADDR " %x %s\n", pCode, codeSize, name.GetUTF8());

        // Write the line.
        if(s_Current != nullptr)
        {
            s_Current->WriteLine(line);
        }
        PAL_PerfJitDump_LogMethod((void*)pCode, codeSize, name.GetUTF8(), nullptr, nullptr);
    }
    EX_CATCH{} EX_END_CATCH(SwallowAllExceptions);
}

void PerfMap::GetNativeImageSignature(PEAssembly * pPEAssembly, CHAR * pszSig, unsigned int nSigSize)
{
    CONTRACTL{
        PRECONDITION(pPEAssembly != nullptr);
        PRECONDITION(pszSig != nullptr);
        PRECONDITION(nSigSize >= GUID_STR_BUFFER_LEN);
    } CONTRACTL_END;

    // We use the MVID as the signature, since ready to run images
    // don't have a native image signature.
    GUID mvid;
    pPEAssembly->GetMVID(&mvid);
    if(!GuidToLPSTR(mvid, pszSig, nSigSize))
    {
        pszSig[0] = '\0';
    }
}
#endif // FEATURE_PERFMAP && !DACCESS_COMPILE
