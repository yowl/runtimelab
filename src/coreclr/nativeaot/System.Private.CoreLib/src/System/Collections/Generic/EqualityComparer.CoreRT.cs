// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#nullable enable
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;

using Internal.IntrinsicSupport;
using Internal.Runtime.CompilerServices;

namespace System.Collections.Generic
{
    internal static class X
    {
        [DllImport("*")]
        internal static unsafe extern int printf(byte* str, byte* unused);
        private static unsafe void PrintString(string s)
        {
            int length = s.Length;
            fixed (char* curChar = s)
            {
                for (int i = 0; i < length; i++)
                {
                    AppContext.X.TwoByteStr curCharStr = new AppContext.X.TwoByteStr();
                    curCharStr.first = (byte)(*(curChar + i));
                    printf((byte*)&curCharStr, null);
                }
            }
        }

        internal static void PrintLine(string s)
        {
            PrintString(s);
            PrintString("\n");
        }

        public static unsafe void PrintLong(long l)
        {
            PrintByte((byte)((l >> 56) & 0xff));
            PrintByte((byte)((l >> 48) & 0xff));
            PrintByte((byte)((l >> 40) & 0xff));
            PrintByte((byte)((l >> 32) & 0xff));
            PrintByte((byte)((l >> 24) & 0xff));
            PrintByte((byte)((l >> 16) & 0xff));
            PrintByte((byte)((l >> 8) & 0xff));
            PrintByte((byte)(l & 0xff));
            PrintString("\n");
        }

        public static unsafe void PrintUint(int l)
        {
            PrintByte((byte)((l >> 24) & 0xff));
            PrintByte((byte)((l >> 16) & 0xff));
            PrintByte((byte)((l >> 8) & 0xff));
            PrintByte((byte)(l & 0xff));

            PrintString("\n");
        }

        public static unsafe void PrintByte(byte b)
        {
            fixed (AppContext.X.TwoByteStr* s = &tbs)
            {
                var nib = (b & 0xf0) >> 4;
                tbs.first = (byte)((nib <= 9 ? '0' : 'A') + (nib <= 9 ? nib : nib - 10));
                printf((byte*)s, null);
                nib = (b & 0xf);
                tbs.first = (byte)((nib <= 9 ? '0' : 'A') + (nib <= 9 ? nib : nib - 10));
                printf((byte*)s, null);
            }
        }

        static AppContext.X.TwoByteStr tbs;

        public struct TwoByteStr
        {
            public byte first;
            public byte second;
        }

    }


    public abstract partial class EqualityComparer<T> : IEqualityComparer, IEqualityComparer<T>
    {
        private static EqualityComparer<T> s_default;

        // The AOT compiler can flip this to false under certain circumstances.
        private static bool SupportsGenericIEquatableInterfaces => true;

        [Intrinsic]
        private static EqualityComparer<T> Create()
        {
            X.PrintLine("EqualityComparer<T> Create()");
            // The compiler will overwrite the Create method with optimized
            // instantiation-specific implementation.
            // This body serves as a fallback when instantiation-specific implementation is unavailable.
            // If that happens, the compiler ensures we generate data structures to make the fallback work
            // when this method is compiled.
            Interlocked.CompareExchange(ref s_default,
                SupportsGenericIEquatableInterfaces
                ? Unsafe.As<EqualityComparer<T>>(EqualityComparerHelpers.GetComparer(typeof(T).TypeHandle))
                : new ObjectEqualityComparer<T>(),
                null);
            return s_default;
        }

        public static EqualityComparer<T> Default
        {
            [Intrinsic]
            get
            {
                // Lazy initialization produces smaller code for CoreRT than initialization in constructor
                return s_default ?? Create();
            }
        }
    }

    public sealed partial class EnumEqualityComparer<T> : EqualityComparer<T> where T : struct, Enum
    {
        public sealed override bool Equals(T x, T y)
        {
            return EqualityComparerHelpers.EnumOnlyEquals(x, y);
        }
    }
}
