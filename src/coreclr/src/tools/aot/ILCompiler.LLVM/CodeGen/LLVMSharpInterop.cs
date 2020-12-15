using System;
using System.Runtime.InteropServices;
using System.Text;
using LLVMSharp.Interop;

namespace Internal.IL
{
    internal class LLVMSharpInterop
    {
        /// <summary>
        /// Workaround while waiting for https://github.com/microsoft/LLVMSharp/pull/141
        /// </summary>
        internal static unsafe uint ElementAtOffset(LLVMTargetDataRef targetDataRef, LLVMTypeRef structTypeRef, ulong offset)
        {
            return LLVM.ElementAtOffset(targetDataRef, structTypeRef, offset);
        }

        ///
        /// Wrapper while waiting for https://github.com/microsoft/LLVMSharp/pull/144
        /// 
        internal static unsafe void DISetSubProgram(LLVMValueRef function, LLVMMetadataRef diFunction)
        {
            LLVM.SetSubprogram(function, diFunction);
        }

        // [DllImport("libLLVM.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "LLVMDIBuilderCreateLocalVariable", ExactSpelling = true)]
        // public static extern unsafe LLVMOpaqueMetadata* DIBuilderCreateLocalVariable(LLVMContext &VMContext,
        // IntPtr PreservedVariables,
        //     LLVMOpaqueMetadata* Scope, sbyte* Name, uint ArgNo, LLVMOpaqueMetadata* File,
        // uint LineNo, LLVMOpaqueMetadata* Ty, 
        //     bool AlwaysPreserve, 
        //         LLVMDIFlags Flags, 
        //     uint AlignInBits);
        //
        //
        internal static unsafe LLVMMetadataRef DIBuilderCreateAutoVariable(LLVMDIBuilderRef diBuilder, LLVMMetadataRef scope, string name,
            LLVMMetadataRef file, uint LineNo, LLVMMetadataRef type, int alwaysPreserve, LLVMDIFlags flags,
            uint alignInBits)
        {
            using var marshaledName = new MarshaledString(name.AsSpan());
            var nameLength = (uint)marshaledName.Length;
            return LLVM.DIBuilderCreateAutoVariable(diBuilder, scope, marshaledName, (UIntPtr)nameLength, file, LineNo, type, alwaysPreserve, flags, alignInBits);
        }

        // [DllImport(LibraryPath, CallingConvention = CallingConvention.Cdecl, EntryPoint = "LLVMDIBuilderCreateBasicType", ExactSpelling = true)]
        // [return: NativeTypeName("LLVMMetadataRef")]
        // public static extern LLVMOpaqueMetadata* DIBuilderCreateBasicType([NativeTypeName("LLVMDIBuilderRef")] LLVMOpaqueDIBuilder* Builder, [NativeTypeName("const char *")] sbyte* Name, [NativeTypeName("size_t")] UIntPtr NameLen,
        // [NativeTypeName("uint64_t")] ulong SizeInBits, [NativeTypeName("LLVMDWARFTypeEncoding")] uint Encoding, LLVMDIFlags Flags);

        internal static unsafe LLVMMetadataRef DIBuilderCreateBasicType(LLVMDIBuilderRef diBuilder, string name, uint sizeInBits, uint encoding, LLVMDIFlags flags)
        {
            using var marshaledName = new MarshaledString(name.AsSpan());
            var nameLength = (uint)marshaledName.Length;
            return LLVM.DIBuilderCreateBasicType(diBuilder, marshaledName, (UIntPtr)nameLength, sizeInBits, encoding, flags);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="diBuilder"></param>
        /// <param name="storage"></param>
        /// <param name="varInfo">expression used to refine the address being declared</param>
        /// <returns></returns>
        internal static unsafe LLVMValueRef DIBuilderInsertDeclareAtEnd(LLVMDIBuilderRef diBuilder, LLVMValueRef storage, LLVMMetadataRef varInfo, LLVMMetadataRef expr, LLVMMetadataRef debugLoc, LLVMBasicBlockRef block)
        {
            return LLVM.DIBuilderInsertDeclareAtEnd(diBuilder, storage, varInfo, expr, debugLoc, block);
        }

        // [DllImport(LibraryPath, CallingConvention = CallingConvention.Cdecl, EntryPoint = "LLVMDIBuilderCreateExpression", ExactSpelling = true)]
        // [return: NativeTypeName("LLVMMetadataRef")]
        // public static extern LLVMOpaqueMetadata* DIBuilderCreateExpression([NativeTypeName("LLVMDIBuilderRef")] LLVMOpaqueDIBuilder* Builder, [NativeTypeName("int64_t *")] long* Addr, [NativeTypeName("size_t")] UIntPtr Length);
        internal static unsafe LLVMMetadataRef DIBuilderCreateExpression(LLVMDIBuilderRef diBuilder, IntPtr addr, uint length)
        {
            return LLVM.DIBuilderCreateExpression(diBuilder, (long*)addr, (UIntPtr)length);
        }

        // [DllImport(LibraryPath, CallingConvention = CallingConvention.Cdecl, EntryPoint = "LLVMInstructionSetDebugLoc", ExactSpelling = true)]
        // public static extern void InstructionSetDebugLoc([NativeTypeName("LLVMValueRef")] LLVMOpaqueValue* Inst, [NativeTypeName("LLVMMetadataRef")] LLVMOpaqueMetadata* Loc);
        internal static unsafe void InstructionSetDebugLoc(LLVMValueRef inst, LLVMMetadataRef loc)
        {
            LLVM.InstructionSetDebugLoc(inst, loc);
        }
    }

    // Some classes copied from LLVMSharp
    internal unsafe struct MarshaledString : IDisposable
    {
        public MarshaledString(ReadOnlySpan<char> input)
        {
            if (input.IsEmpty)
            {
                var value = Marshal.AllocHGlobal(1);
                Marshal.WriteByte(value, 0, 0);

                Length = 0;
                Value = (sbyte*)value;
            }
            else
            {
                var valueBytes = Encoding.UTF8.GetBytes(input.ToString());
                var length = valueBytes.Length;
                var value = Marshal.AllocHGlobal(length + 1);
                Marshal.Copy(valueBytes, 0, value, length);
                Marshal.WriteByte(value, length, 0);

                Length = length;
                Value = (sbyte*)value;
            }
        }

        public int Length { get; private set; }

        public sbyte* Value { get; private set; }

        public void Dispose()
        {
            if (Value != null)
            {
                Marshal.FreeHGlobal((IntPtr)Value);
                Value = null;
                Length = 0;
            }
        }

        public static implicit operator sbyte*(in MarshaledString value)
        {
            return value.Value;
        }

        public override string ToString()
        {
            var span = new ReadOnlySpan<byte>(Value, Length);
            return span.AsString();
        }
    }

    internal static unsafe class SpanExtensions
    {
        public static string AsString(this Span<byte> self) => AsString((ReadOnlySpan<byte>)self);

        public static string AsString(this ReadOnlySpan<byte> self)
        {
            if (self.IsEmpty)
            {
                return string.Empty;
            }

            fixed (byte* pSelf = self)
            {
                return Encoding.UTF8.GetString(pSelf, self.Length);
            }
        }
    }


    // from http://dwarfstd.org/Download.php
    internal enum DW_ATE_Types
    {
        DW_ATE_address, // linear machine address (for segmented addresses see Section 2.12)
        DW_ATE_boolean, // true or false 
        DW_ATE_complex_float, // complex binary floating-point number 
        DW_ATE_float, // binary floating-point number 
        DW_ATE_imaginary_float, // imaginary binary floating-point number   
        DW_ATE_signed, // signed binary integer 
        DW_ATE_signed_char, // signed character 
        DW_ATE_unsigned, // unsigned binary integer 
        DW_ATE_unsigned_char, // unsigned character 
        DW_ATE_packed_decimal, // packed decimal
        DW_ATE_numeric_string, // numeric string
        DW_ATE_edited, // edited string
        DW_ATE_signed_fixed, // signed fixed-point scaled integer
        DW_ATE_unsigned_fixed, // unsigned fixed-point scaled integer
        DW_ATE_decimal_float, // decimal floating-point number
        DW_ATE_UTF // Unicode character
    }
}
