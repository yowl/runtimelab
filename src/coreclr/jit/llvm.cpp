// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#ifdef TARGET_WASM
#include <string.h>
#include "compiler.h"
#include "block.h"
#include "gentree.h"
#include "llvm.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Bitcode/BitcodeWriter.h"

using llvm::Function;
using llvm::FunctionType;
using llvm::Type;
using llvm::LLVMContext;
using llvm::ArrayRef;
using llvm::Module;

static Module* _module;
static LLVMContext _llvmContext;
static void* _thisPtr;
static const char* (*_getMangledMethodName)(void*, CORINFO_METHOD_STRUCT_*);
static char* _outputFileName;
static Function* _doNothingFunction;

Compiler* _compiler;
Compiler::Info _info;

extern "C" DLLEXPORT void registerLlvmCallbacks(void* thisPtr, const char* outputFileName, const char* triple, const char* dataLayout, const char* (*getMangledMethodNamePtr)(void*, CORINFO_METHOD_STRUCT_*))
{
    _thisPtr = thisPtr;
    _getMangledMethodName = getMangledMethodNamePtr;
    _module = new Module(llvm::StringRef("netscripten-clrjit"), _llvmContext);
    _module->setTargetTriple(triple);
    _module->setDataLayout(dataLayout);

//    _outputFileName = getAllocator(CMK_DebugOnly).allocate<char>(strlen(outputFileName) + 1)
    _outputFileName = (char*)malloc(strlen(outputFileName) + 7);
    strcpy(_outputFileName, "1.txt"); // ??? without this _outputFileName is corrupted
    strcpy(_outputFileName, outputFileName);
    strcpy(_outputFileName + strlen(_outputFileName) - 3, "clrjit"); // use different module output name for now, TODO: delete if old LLVM gen does not create a module
    strcat(_outputFileName, ".bc");
}

void Llvm::Init()
{
}

void Llvm::llvmShutdown()
{
#if DEBUG
    std::error_code ec;
    char* txtFileName = (char *)malloc(strlen(_outputFileName) + 2); // .txt is longer than .bc
    strcpy(txtFileName, _outputFileName);
    strcpy(txtFileName + strlen(_outputFileName) - 2, "txt");
    llvm::raw_fd_ostream textOutputStream(txtFileName, ec);
    _module->print(textOutputStream, (llvm::AssemblyAnnotationWriter*)NULL);
    free(txtFileName);
#endif //DEBUG
    llvm::raw_fd_ostream OS(_outputFileName, ec);
    llvm::WriteBitcodeToFile(*_module, OS);
    delete _module;
//    Module.Verify(LLVMVerifierFailureAction.LLVMAbortProcessAction);
}

FunctionType* GetFunctionTypeForMethod(Compiler::Info info)
{
    _compiler->eeGetCallSiteSig(_compiler->eeTryResolveToken() pResolvedToken->token, pResolvedToken->tokenScope, pResolvedToken->tokenContext, sig);

    bool hasHiddenParam =

    internal static LLVMTypeRef GetLLVMSignatureForMethod(MethodSignature signature, bool hasHiddenParam)
    {
        TypeDesc returnType = signature.ReturnType;
        LLVMTypeRef llvmReturnType;
        bool returnOnStack = false;
        if (!NeedsReturnStackSlot(signature))
        {
            returnOnStack = true;
            llvmReturnType = ILImporter.GetLLVMTypeForTypeDesc(returnType);
        }
        else
        {
            llvmReturnType = LLVMTypeRef.Void;
        }

        List<LLVMTypeRef> signatureTypes = new List<LLVMTypeRef>();
        signatureTypes.Add(LLVMTypeRef.CreatePointer(LLVMTypeRef.Int8, 0)); // Shadow stack pointer

        if (!returnOnStack && !signature.ReturnType.IsVoid)
        {
            signatureTypes.Add(LLVMTypeRef.CreatePointer(LLVMTypeRef.Int8, 0));
        }

        if (hasHiddenParam)
        {
            signatureTypes.Add(LLVMTypeRef.CreatePointer(LLVMTypeRef.Int8, 0)); // *EEType
        }

        // Intentionally skipping the 'this' pointer since it could always be a GC reference
        // and thus must be on the shadow stack
        foreach(TypeDesc type in signature)
        {
            if (ILImporter.CanStoreTypeOnStack(type))
            {
                signatureTypes.Add(ILImporter.GetLLVMTypeForTypeDesc(type));
            }
        }

        return LLVMTypeRef.CreateFunction(llvmReturnType, signatureTypes.ToArray(), false);
    }

    if (info.compArgsCount != 0 || info.compRetType != TYP_VOID)
    {
        fatal(CORJIT_SKIPPED);
    }
    // all managed functions have shadow stack as first arg (i8*)
    return FunctionType::get(Type::getVoidTy(_llvmContext), ArrayRef<Type*>(Type::getInt8PtrTy(_llvmContext)), false);
}

void EmitDoNothingCall(llvm::IRBuilder<>& builder)
{
    if (_doNothingFunction == nullptr)
    {
        _doNothingFunction = Function::Create(FunctionType::get(Type::getVoidTy(_llvmContext), ArrayRef<Type*>(), false), Function::ExternalLinkage, 0U, "llvm.donothing", _module);
    }
    builder.CreateCall(_doNothingFunction);
}

bool visitNode(llvm::IRBuilder<> &builder, GenTree* node)
{
    switch (node->OperGet())
    {
        case GT_IL_OFFSET:
            break;
        case GT_NO_OP:
            EmitDoNothingCall(builder);
            break;
        case GT_RETURN:
            builder.CreateRetVoid();
            break;
        default:
             return false;
    }
    return true;
}

Function* CreateLLVMFunction(const char* mangledName, Compiler::Info& info)
{
    info.c
    return Function::Create(GetFunctionTypeForMethod(info), Function::ExternalLinkage, 0U, mangledName, _module); // TODO: ExternalLinkage forced as linked from old module
}

Function* GetOrCreateLLVMFunction(const char* mangledName, Compiler::Info& info)
{
    Function* llvmFunction = _module->getFunction(mangledName);

    if (llvmFunction == nullptr)
    {
        return CreateLLVMFunction(mangledName, signature, hasHiddenParam);
    }
    return llvmFunction;
}

//------------------------------------------------------------------------
// Compile: Compile IR to LLVM, adding to the LLVM Module
//
void Llvm::Compile(Compiler* pCompiler)
{
    _compiler = pCompiler;
    _info = pCompiler->info;

    const char* mangledName = (*_getMangledMethodName)(_thisPtr, _info.compMethodHnd);
    Function* function = GetOrCreateLLVMFunction(mangledName, info);

    BasicBlock* firstBb = pCompiler->fgFirstBB;
    llvm::IRBuilder<> builder(_llvmContext);
    for (BasicBlock* block = firstBb; block; block = block->bbNext)
    {
        if (block->hasTryIndex())
        {
            function->dropAllReferences();
            function->eraseFromParent();
            fatal(CORJIT_SKIPPED); // TODO: skip anything with a try block for now
        }

        llvm::BasicBlock* entry = llvm::BasicBlock::Create(_llvmContext, "", function);
        builder.SetInsertPoint(entry);
  //      GenTree* firstGt = block->GetFirstLIRNode();
//        firstGt->VisitOperands();
        for (GenTree* node = block->GetFirstLIRNode(); node; node = node->gtNext)
        {
            if (!visitNode(builder, node))
            {
                // delete created function , dont want duplicate symbols
                function->dropAllReferences();
                function->eraseFromParent();
                fatal(CORJIT_SKIPPED); // visitNode incomplete
            }
        }
    }
}
#endif
