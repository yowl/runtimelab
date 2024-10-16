// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// Allow multiple inclusion.

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
//
// The JIT/EE interface is versioned. By "interface", we mean mean any and all communication between the
// JIT and the EE. Any time a change is made to the interface, the JIT/EE interface version identifier
// must be updated. See code:JITEEVersionIdentifier for more information.
//
// THIS FILE IS PART OF THE JIT-EE INTERFACE.
//
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef DYNAMICJITHELPER
//I should never try to generate an alignment stub for a dynamic helper
#define DYNAMICJITHELPER(code,fn,binderId) JITHELPER(code,fn,binderId)
#endif

// pfnHelper is set to NULL if it is a stubbed helper.
// It will be set in InitJITHelpers2

    JITHELPER(CORINFO_HELP_UNDEF,               NULL,               METHOD__NIL)

    // Arithmetic
    JITHELPER(CORINFO_HELP_DIV,                 JIT_Div,            METHOD__NIL)
    JITHELPER(CORINFO_HELP_MOD,                 JIT_Mod,            METHOD__NIL)
    JITHELPER(CORINFO_HELP_UDIV,                JIT_UDiv,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_UMOD,                JIT_UMod,           METHOD__NIL)

    // CORINFO_HELP_DBL2INT, CORINFO_HELP_DBL2UINT, and CORINFO_HELP_DBL2LONG get
    // patched for CPUs that support SSE2 (P4 and above).
#ifndef TARGET_64BIT
    JITHELPER(CORINFO_HELP_LLSH,                JIT_LLsh,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_LRSH,                JIT_LRsh,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_LRSZ,                JIT_LRsz,           METHOD__NIL)
#else // !TARGET_64BIT
    JITHELPER(CORINFO_HELP_LLSH,                NULL,               METHOD__NIL)
    JITHELPER(CORINFO_HELP_LRSH,                NULL,               METHOD__NIL)
    JITHELPER(CORINFO_HELP_LRSZ,                NULL,               METHOD__NIL)
#endif // TARGET_64BIT
    JITHELPER(CORINFO_HELP_LMUL,                JIT_LMul,           METHOD__NIL)
#ifndef TARGET_64BIT
    DYNAMICJITHELPER(CORINFO_HELP_LMUL_OVF,     NULL,               METHOD__MATH__MULTIPLY_CHECKED_INT64)
    DYNAMICJITHELPER(CORINFO_HELP_ULMUL_OVF,    NULL,               METHOD__MATH__MULTIPLY_CHECKED_UINT64)
#else
    DYNAMICJITHELPER(CORINFO_HELP_LMUL_OVF,     NULL,               METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ULMUL_OVF,    NULL,               METHOD__NIL)
#endif // TARGET_64BIT
    JITHELPER(CORINFO_HELP_LDIV,                JIT_LDiv,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_LMOD,                JIT_LMod,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_ULDIV,               JIT_ULDiv,          METHOD__NIL)
    JITHELPER(CORINFO_HELP_ULMOD,               JIT_ULMod,          METHOD__NIL)
    JITHELPER(CORINFO_HELP_LNG2DBL,             JIT_Lng2Dbl,        METHOD__NIL)
    JITHELPER(CORINFO_HELP_ULNG2DBL,            JIT_ULng2Dbl,       METHOD__NIL)
    JITHELPER(CORINFO_HELP_DBL2INT,             JIT_Dbl2Int,        METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_DBL2INT_OVF,  NULL,               METHOD__MATH__CONVERT_TO_INT32_CHECKED)
    JITHELPER(CORINFO_HELP_DBL2LNG,             JIT_Dbl2Lng,        METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_DBL2LNG_OVF,  NULL,               METHOD__MATH__CONVERT_TO_INT64_CHECKED)
    JITHELPER(CORINFO_HELP_DBL2UINT,            JIT_Dbl2UInt,       METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_DBL2UINT_OVF, NULL,               METHOD__MATH__CONVERT_TO_UINT32_CHECKED)
    JITHELPER(CORINFO_HELP_DBL2ULNG,            JIT_Dbl2ULng,       METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_DBL2ULNG_OVF, NULL,               METHOD__MATH__CONVERT_TO_UINT64_CHECKED)
    JITHELPER(CORINFO_HELP_FLTREM,              JIT_FltRem,         METHOD__NIL)
    JITHELPER(CORINFO_HELP_DBLREM,              JIT_DblRem,         METHOD__NIL)

    // Allocating a new object
    JITHELPER(CORINFO_HELP_NEWFAST,                     JIT_New,    METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWFAST_MAYBEFROZEN,         JIT_NewMaybeFrozen,METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_NEWSFAST,             JIT_New,    METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWSFAST_FINALIZE,           NULL,       METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_NEWSFAST_ALIGN8,      JIT_New,    METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWSFAST_ALIGN8_VC,          NULL,       METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWSFAST_ALIGN8_FINALIZE,    NULL,       METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEW_MDARR,                   JIT_NewMDArr,METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEW_MDARR_RARE,              JIT_NewMDArr,METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWARR_1_DIRECT,             JIT_NewArr1,METHOD__NIL)
    JITHELPER(CORINFO_HELP_NEWARR_1_MAYBEFROZEN,        JIT_NewArr1MaybeFrozen,METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_NEWARR_1_OBJ,         JIT_NewArr1,METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_NEWARR_1_VC,          JIT_NewArr1,METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_NEWARR_1_ALIGN8,      JIT_NewArr1,METHOD__NIL)

    JITHELPER(CORINFO_HELP_STRCNS,              JIT_StrCns,         METHOD__NIL)

    // Object model
    JITHELPER(CORINFO_HELP_INITCLASS,           JIT_InitClass,      METHOD__NIL)
    JITHELPER(CORINFO_HELP_INITINSTCLASS,       JIT_InitInstantiatedClass, METHOD__NIL)

    // Casting helpers
    DYNAMICJITHELPER(CORINFO_HELP_ISINSTANCEOFINTERFACE,    NULL,   METHOD__CASTHELPERS__ISINSTANCEOFINTERFACE)
    DYNAMICJITHELPER(CORINFO_HELP_ISINSTANCEOFARRAY,        NULL,   METHOD__CASTHELPERS__ISINSTANCEOFANY) // array cast uses the "ANY" helper
    DYNAMICJITHELPER(CORINFO_HELP_ISINSTANCEOFCLASS,        NULL,   METHOD__CASTHELPERS__ISINSTANCEOFCLASS)
    DYNAMICJITHELPER(CORINFO_HELP_ISINSTANCEOFANY,          NULL,   METHOD__CASTHELPERS__ISINSTANCEOFANY)
    DYNAMICJITHELPER(CORINFO_HELP_CHKCASTINTERFACE,         NULL,   METHOD__CASTHELPERS__CHKCASTINTERFACE)
    DYNAMICJITHELPER(CORINFO_HELP_CHKCASTARRAY,             NULL,   METHOD__CASTHELPERS__CHKCASTANY) // array cast uses the "ANY" helper
    DYNAMICJITHELPER(CORINFO_HELP_CHKCASTCLASS,             NULL,   METHOD__CASTHELPERS__CHKCASTCLASS)
    DYNAMICJITHELPER(CORINFO_HELP_CHKCASTANY,               NULL,   METHOD__CASTHELPERS__CHKCASTANY)
    DYNAMICJITHELPER(CORINFO_HELP_CHKCASTCLASS_SPECIAL,     NULL,   METHOD__CASTHELPERS__CHKCASTCLASSSPECIAL)

    JITHELPER(CORINFO_HELP_ISINSTANCEOF_EXCEPTION, JIT_IsInstanceOfException, METHOD__NIL)

    DYNAMICJITHELPER(CORINFO_HELP_BOX,          JIT_Box,            METHOD__NIL)
    JITHELPER(CORINFO_HELP_BOX_NULLABLE,        JIT_Box,            METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_UNBOX,        NULL,               METHOD__CASTHELPERS__UNBOX)
    JITHELPER(CORINFO_HELP_UNBOX_TYPETEST,      JIT_Unbox_TypeTest, METHOD__NIL)
    JITHELPER(CORINFO_HELP_UNBOX_NULLABLE,      JIT_Unbox_Nullable, METHOD__NIL)

    JITHELPER(CORINFO_HELP_GETREFANY,           JIT_GetRefAny,      METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ARRADDR_ST,   NULL,               METHOD__CASTHELPERS__STELEMREF)
    DYNAMICJITHELPER(CORINFO_HELP_LDELEMA_REF,  NULL,               METHOD__CASTHELPERS__LDELEMAREF)

    // Exceptions
    DYNAMICJITHELPER(CORINFO_HELP_THROW,        IL_Throw,           METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_RETHROW,      IL_Rethrow,         METHOD__NIL)
    JITHELPER(CORINFO_HELP_USER_BREAKPOINT,     JIT_UserBreakpoint, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_RNGCHKFAIL,   NULL,               METHOD__THROWHELPERS__THROWINDEXOUTOFRANGEEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_OVERFLOW,     NULL,               METHOD__THROWHELPERS__THROWOVERFLOWEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROWDIVZERO, NULL,               METHOD__THROWHELPERS__THROWDIVIDEBYZEROEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROWNULLREF, NULL,               METHOD__THROWHELPERS__THROWNULLREFEXCEPTION)
    JITHELPER(CORINFO_HELP_THROWMISALIGN,       NULL,               METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_VERIFICATION, NULL,               METHOD__THROWHELPERS__THROWVERIFICATIONEXCEPTION)
    JITHELPER(CORINFO_HELP_FAIL_FAST,           JIT_FailFast,       METHOD__NIL)
    JITHELPER(CORINFO_HELP_METHOD_ACCESS_EXCEPTION,JIT_ThrowMethodAccessException, METHOD__NIL)
    JITHELPER(CORINFO_HELP_FIELD_ACCESS_EXCEPTION,JIT_ThrowFieldAccessException, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CLASS_ACCESS_EXCEPTION,JIT_ThrowClassAccessException, METHOD__NIL)

#ifdef FEATURE_EH_FUNCLETS
    JITHELPER(CORINFO_HELP_ENDCATCH,            NULL,               METHOD__NIL)
#else
    JITHELPER(CORINFO_HELP_ENDCATCH,            JIT_EndCatch,       METHOD__NIL)
#endif

    JITHELPER(CORINFO_HELP_MON_ENTER,               JIT_MonEnterWorker, METHOD__NIL)
    JITHELPER(CORINFO_HELP_MON_EXIT,                JIT_MonExitWorker, METHOD__NIL)

    JITHELPER(CORINFO_HELP_GETCLASSFROMMETHODPARAM, JIT_GetClassFromMethodParam, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETSYNCFROMCLASSHANDLE,  JIT_GetRuntimeType, METHOD__NIL)

    // GC support
    DYNAMICJITHELPER(CORINFO_HELP_STOP_FOR_GC,  JIT_RareDisableHelper,  METHOD__NIL)
    JITHELPER(CORINFO_HELP_POLL_GC,             JIT_PollGC,         METHOD__NIL)
    JITHELPER(CORINFO_HELP_STRESS_GC,           JIT_StressGC,       METHOD__NIL)

    JITHELPER(CORINFO_HELP_CHECK_OBJ,           JIT_CheckObj,       METHOD__NIL)

    // GC Write barrier support
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF,   JIT_WriteBarrier,   METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF, JIT_CheckedWriteBarrier,METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_ENSURE_NONHEAP, JIT_WriteBarrierEnsureNonHeapTarget,METHOD__NIL)

    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_BYREF, JIT_ByRefWriteBarrier,METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_BULK_WRITEBARRIER, NULL, METHOD__BUFFER__MEMCOPYGC)

    // Accessing fields

    JITHELPER(CORINFO_HELP_GETFIELDADDR,                JIT_GetFieldAddr,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETSTATICFIELDADDR,          JIT_GetStaticFieldAddr,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETSTATICFIELDADDR_TLS,      NULL,       METHOD__NIL)

    JITHELPER(CORINFO_HELP_GET_GCSTATIC_BASE,                JIT_GetGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_NONGCSTATIC_BASE,             JIT_GetNonGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_GCSTATIC_BASE,         JIT_GetDynamicGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCSTATIC_BASE,      JIT_GetDynamicNonGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETPINNED_GCSTATIC_BASE,          JIT_GetDynamicGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETPINNED_NONGCSTATIC_BASE,       JIT_GetDynamicNonGCStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_GCSTATIC_BASE_NOCTOR,         JIT_GetGCStaticBaseNoCtor, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_NONGCSTATIC_BASE_NOCTOR,      JIT_GetNonGCStaticBaseNoCtor, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_GCSTATIC_BASE_NOCTOR,   JIT_GetDynamicGCStaticBaseNoCtor, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCSTATIC_BASE_NOCTOR,JIT_GetDynamicNonGCStaticBaseNoCtor, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETPINNED_GCSTATIC_BASE_NOCTOR,   JIT_GetDynamicGCStaticBaseNoCtor, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETPINNED_NONGCSTATIC_BASE_NOCTOR,JIT_GetDynamicNonGCStaticBaseNoCtor, METHOD__NIL)

    // Thread statics
    JITHELPER(CORINFO_HELP_GET_GCTHREADSTATIC_BASE,                 JIT_GetGCThreadStaticBase,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_NONGCTHREADSTATIC_BASE,              JIT_GetNonGCThreadStaticBase,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_GCTHREADSTATIC_BASE,          JIT_GetDynamicGCThreadStaticBase,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCTHREADSTATIC_BASE,       JIT_GetDynamicNonGCThreadStaticBase,METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_GCTHREADSTATIC_BASE_NOCTOR,          JIT_GetGCThreadStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GET_NONGCTHREADSTATIC_BASE_NOCTOR,       JIT_GetNonGCThreadStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_GCTHREADSTATIC_BASE_NOCTOR,   JIT_GetDynamicGCThreadStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCTHREADSTATIC_BASE_NOCTOR,JIT_GetDynamicNonGCThreadStaticBase, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_GCTHREADSTATIC_BASE_NOCTOR_OPTIMIZED,JIT_GetGCThreadStaticBaseOptimized, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCTHREADSTATIC_BASE_NOCTOR_OPTIMIZED,JIT_GetNonGCThreadStaticBaseOptimized, METHOD__NIL)
    JITHELPER(CORINFO_HELP_GETDYNAMIC_NONGCTHREADSTATIC_BASE_NOCTOR_OPTIMIZED2,JIT_GetNonGCThreadStaticBaseOptimized2, METHOD__NIL)

    // Debugger
    JITHELPER(CORINFO_HELP_DBG_IS_JUST_MY_CODE, JIT_DbgIsJustMyCode,METHOD__NIL)

    /* Profiling enter/leave probe addresses */
    DYNAMICJITHELPER(CORINFO_HELP_PROF_FCN_ENTER,    JIT_ProfilerEnterLeaveTailcallStub, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_PROF_FCN_LEAVE,    JIT_ProfilerEnterLeaveTailcallStub, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_PROF_FCN_TAILCALL, JIT_ProfilerEnterLeaveTailcallStub, METHOD__NIL)

    // Miscellaneous
    JITHELPER(CORINFO_HELP_PINVOKE_CALLI,       GenericPInvokeCalliHelper, METHOD__NIL)

#if defined(TARGET_X86) && !defined(UNIX_X86_ABI)
    JITHELPER(CORINFO_HELP_TAILCALL,            JIT_TailCall,             METHOD__NIL)
#else
    JITHELPER(CORINFO_HELP_TAILCALL,            NULL,                     METHOD__NIL)
#endif

    JITHELPER(CORINFO_HELP_GETCURRENTMANAGEDTHREADID,  JIT_GetCurrentManagedThreadId, METHOD__NIL)

#ifdef TARGET_64BIT
    JITHELPER(CORINFO_HELP_INIT_PINVOKE_FRAME,  JIT_InitPInvokeFrame,  METHOD__NIL)
#else
    DYNAMICJITHELPER(CORINFO_HELP_INIT_PINVOKE_FRAME,  NULL,        METHOD__NIL)
#endif

    DYNAMICJITHELPER(CORINFO_HELP_MEMSET, NULL, METHOD__SPAN_HELPERS__MEMSET)
    DYNAMICJITHELPER(CORINFO_HELP_MEMZERO, NULL, METHOD__SPAN_HELPERS__MEMZERO)
    DYNAMICJITHELPER(CORINFO_HELP_MEMCPY, NULL, METHOD__SPAN_HELPERS__MEMCOPY)
    JITHELPER(CORINFO_HELP_NATIVE_MEMSET, Jit_NativeMemSet, METHOD__NIL)

    // Generics
    DYNAMICJITHELPER(CORINFO_HELP_RUNTIMEHANDLE_METHOD, NULL, METHOD__GENERICSHELPERS__METHOD)
    DYNAMICJITHELPER(CORINFO_HELP_RUNTIMEHANDLE_CLASS, NULL, METHOD__GENERICSHELPERS__CLASS)
    JITHELPER(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPE, JIT_GetRuntimeType,           METHOD__NIL)
    JITHELPER(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPE_MAYBENULL, JIT_GetRuntimeType_MaybeNull, METHOD__NIL)
    JITHELPER(CORINFO_HELP_METHODDESC_TO_STUBRUNTIMEMETHOD, JIT_GetRuntimeMethodStub,METHOD__NIL)
    JITHELPER(CORINFO_HELP_FIELDDESC_TO_STUBRUNTIMEFIELD, JIT_GetRuntimeFieldStub,  METHOD__NIL)
    JITHELPER(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE, JIT_GetRuntimeType, METHOD__NIL)
    JITHELPER(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE_MAYBENULL, JIT_GetRuntimeType_MaybeNull, METHOD__NIL)

    DYNAMICJITHELPER(CORINFO_HELP_VIRTUAL_FUNC_PTR,     NULL,   METHOD__VIRTUALDISPATCHHELPERS__VIRTUALFUNCTIONPOINTER)

    JITHELPER(CORINFO_HELP_READYTORUN_NEW,                 NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_NEWARR_1,            NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_ISINSTANCEOF,        NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_CHKCAST,             NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_GCSTATIC_BASE,       NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_NONGCSTATIC_BASE,    NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_THREADSTATIC_BASE,   NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_THREADSTATIC_BASE_NOCTOR,   NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_NONGCTHREADSTATIC_BASE, NULL,METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_VIRTUAL_FUNC_PTR,    NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_GENERIC_HANDLE,      NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_DELEGATE_CTOR,       NULL,   METHOD__NIL)
    JITHELPER(CORINFO_HELP_READYTORUN_GENERIC_STATIC_BASE, NULL,   METHOD__NIL)

#ifdef FEATURE_EH_FUNCLETS
    DYNAMICJITHELPER(CORINFO_HELP_EE_PERSONALITY_ROUTINE, ProcessCLRException,               METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_EE_PERSONALITY_ROUTINE_FILTER_FUNCLET, ProcessCLRException,METHOD__NIL)
#else // FEATURE_EH_FUNCLETS
    JITHELPER(CORINFO_HELP_EE_PERSONALITY_ROUTINE, NULL,                              METHOD__NIL)
    JITHELPER(CORINFO_HELP_EE_PERSONALITY_ROUTINE_FILTER_FUNCLET, NULL,               METHOD__NIL)
#endif // !FEATURE_EH_FUNCLETS

#ifdef TARGET_X86
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_EAX, JIT_WriteBarrierEAX, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_EBX, JIT_WriteBarrierEBX, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_ECX, JIT_WriteBarrierECX, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_ESI, JIT_WriteBarrierESI, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_EDI, JIT_WriteBarrierEDI, METHOD__NIL)
    DYNAMICJITHELPER(CORINFO_HELP_ASSIGN_REF_EBP, JIT_WriteBarrierEBP, METHOD__NIL)

    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EAX, JIT_CheckedWriteBarrierEAX, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EBX, JIT_CheckedWriteBarrierEBX, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_ECX, JIT_CheckedWriteBarrierECX, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_ESI, JIT_CheckedWriteBarrierESI, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EDI, JIT_CheckedWriteBarrierEDI, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EBP, JIT_CheckedWriteBarrierEBP, METHOD__NIL)
#else
    JITHELPER(CORINFO_HELP_ASSIGN_REF_EAX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_EBX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_ECX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_ESI, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_EDI, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_ASSIGN_REF_EBP, NULL, METHOD__NIL)

    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EAX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EBX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_ECX, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_ESI, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EDI, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CHECKED_ASSIGN_REF_EBP, NULL, METHOD__NIL)
#endif

    JITHELPER(CORINFO_HELP_LOOP_CLONE_CHOICE_ADDR, JIT_LoopCloneChoiceAddr, METHOD__NIL)
    JITHELPER(CORINFO_HELP_DEBUG_LOG_LOOP_CLONING, JIT_DebugLogLoopCloning, METHOD__NIL)

    DYNAMICJITHELPER(CORINFO_HELP_THROW_ARGUMENTEXCEPTION,              NULL,   METHOD__THROWHELPERS__THROWARGUMENTEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_ARGUMENTOUTOFRANGEEXCEPTION,    NULL,   METHOD__THROWHELPERS__THROWARGUMENTOUTOFRANGEEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_NOT_IMPLEMENTED,                NULL,   METHOD__THROWHELPERS__THROWNOTIMPLEMENTEDEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_PLATFORM_NOT_SUPPORTED,         NULL,   METHOD__THROWHELPERS__THROWPLATFORMNOTSUPPORTEDEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_TYPE_NOT_SUPPORTED,             NULL,   METHOD__THROWHELPERS__THROWTYPENOTSUPPORTED)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_AMBIGUOUS_RESOLUTION_EXCEPTION, NULL,   METHOD__THROWHELPERS__THROWAMBIGUOUSRESOLUTIONEXCEPTION)
    DYNAMICJITHELPER(CORINFO_HELP_THROW_ENTRYPOINT_NOT_FOUND_EXCEPTION, NULL,   METHOD__THROWHELPERS__THROWENTRYPOINTNOTFOUNDEXCEPTION)

    JITHELPER(CORINFO_HELP_JIT_PINVOKE_BEGIN,         JIT_PInvokeBegin,     METHOD__NIL)
    JITHELPER(CORINFO_HELP_JIT_PINVOKE_END,           JIT_PInvokeEnd,       METHOD__NIL)

    JITHELPER(CORINFO_HELP_JIT_REVERSE_PINVOKE_ENTER,                   JIT_ReversePInvokeEnter,                 METHOD__NIL)
    JITHELPER(CORINFO_HELP_JIT_REVERSE_PINVOKE_ENTER_TRACK_TRANSITIONS, JIT_ReversePInvokeEnterTrackTransitions, METHOD__NIL)
    JITHELPER(CORINFO_HELP_JIT_REVERSE_PINVOKE_EXIT,                    JIT_ReversePInvokeExit,                  METHOD__NIL)
    JITHELPER(CORINFO_HELP_JIT_REVERSE_PINVOKE_EXIT_TRACK_TRANSITIONS,  JIT_ReversePInvokeExitTrackTransitions,  METHOD__NIL)

    JITHELPER(CORINFO_HELP_GVMLOOKUP_FOR_SLOT, NULL, METHOD__NIL)

#if !defined(TARGET_ARM64) && !defined(TARGET_LOONGARCH64) && !defined(TARGET_RISCV64)
    JITHELPER(CORINFO_HELP_STACK_PROBE, JIT_StackProbe, METHOD__NIL)
#else
    JITHELPER(CORINFO_HELP_STACK_PROBE, NULL, METHOD__NIL)
#endif

    JITHELPER(CORINFO_HELP_PATCHPOINT, JIT_Patchpoint, METHOD__NIL)
    JITHELPER(CORINFO_HELP_PARTIAL_COMPILATION_PATCHPOINT, JIT_PartialCompilationPatchpoint, METHOD__NIL)

    JITHELPER(CORINFO_HELP_CLASSPROFILE32, JIT_ClassProfile32, METHOD__NIL)
    JITHELPER(CORINFO_HELP_CLASSPROFILE64, JIT_ClassProfile64, METHOD__NIL)
    JITHELPER(CORINFO_HELP_DELEGATEPROFILE32, JIT_DelegateProfile32, METHOD__NIL)
    JITHELPER(CORINFO_HELP_DELEGATEPROFILE64, JIT_DelegateProfile64, METHOD__NIL)
    JITHELPER(CORINFO_HELP_VTABLEPROFILE32, JIT_VTableProfile32, METHOD__NIL)
    JITHELPER(CORINFO_HELP_VTABLEPROFILE64, JIT_VTableProfile64, METHOD__NIL)
    JITHELPER(CORINFO_HELP_COUNTPROFILE32, JIT_CountProfile32, METHOD__NIL)
    JITHELPER(CORINFO_HELP_COUNTPROFILE64, JIT_CountProfile64, METHOD__NIL)
    JITHELPER(CORINFO_HELP_VALUEPROFILE32, JIT_ValueProfile32, METHOD__NIL)
    JITHELPER(CORINFO_HELP_VALUEPROFILE64, JIT_ValueProfile64, METHOD__NIL)

#if defined(TARGET_AMD64) || defined(TARGET_ARM64)
    JITHELPER(CORINFO_HELP_VALIDATE_INDIRECT_CALL, JIT_ValidateIndirectCall, METHOD__NIL)
#ifdef TARGET_AMD64
    DYNAMICJITHELPER(CORINFO_HELP_DISPATCH_INDIRECT_CALL, JIT_DispatchIndirectCall, METHOD__NIL)
#else
    JITHELPER(CORINFO_HELP_DISPATCH_INDIRECT_CALL, JIT_DispatchIndirectCall, METHOD__NIL)
#endif
#else
    JITHELPER(CORINFO_HELP_VALIDATE_INDIRECT_CALL, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_DISPATCH_INDIRECT_CALL, NULL, METHOD__NIL)
#endif

    JITHELPER(CORINFO_HELP_LLVM_GET_OR_INIT_SHADOW_STACK_TOP, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_EH_CATCH, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_EH_POP_UNWOUND_VIRTUAL_FRAMES, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_EH_PUSH_VIRTUAL_UNWIND_FRAME, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_EH_POP_VIRTUAL_UNWIND_FRAME, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_EH_UNHANDLED_EXCEPTION, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_RESOLVE_INTERFACE_CALL_TARGET, NULL, METHOD__NIL)
    JITHELPER(CORINFO_HELP_LLVM_GET_EXTERNAL_CALL_TARGET, NULL, METHOD__NIL)

#undef JITHELPER
#undef DYNAMICJITHELPER
#undef JITHELPER
#undef DYNAMICJITHELPER
