// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Tracing;
using System.Reflection;
using System.Runtime.ExceptionServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Runtime.Versioning;
using System.Threading;

namespace System
{
    public static partial class AppContext
    {
        private static Dictionary<string, object?>? s_dataStore;
        private static Dictionary<string, bool>? s_switches;
        private static string? s_defaultBaseDirectory;

        public static string BaseDirectory =>
            // The value of APP_CONTEXT_BASE_DIRECTORY key has to be a string and it is not allowed to be any other type.
            // Otherwise the caller will get invalid cast exception
            GetData("APP_CONTEXT_BASE_DIRECTORY") as string ??
            (s_defaultBaseDirectory ??= GetBaseDirectoryCore());

        public static string? TargetFrameworkName =>
            // The Target framework is not the framework that the process is actually running on.
            // It is the value read from the TargetFrameworkAttribute on the .exe that started the process.
            Assembly.GetEntryAssembly()?.GetCustomAttribute<TargetFrameworkAttribute>()?.FrameworkName;

        public static object? GetData(string name)
        {
            if (name == null)
                throw new ArgumentNullException(nameof(name));

            if (s_dataStore == null)
                return null;

            object? data;
            lock (s_dataStore)
            {
                s_dataStore.TryGetValue(name, out data);
            }
            return data;
        }

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
                        TwoByteStr curCharStr = new TwoByteStr();
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
                fixed (TwoByteStr* s = &tbs)
                {
                    var nib = (b & 0xf0) >> 4;
                    tbs.first = (byte)((nib <= 9 ? '0' : 'A') + (nib <= 9 ? nib : nib - 10));
                    printf((byte*)s, null);
                    nib = (b & 0xf);
                    tbs.first = (byte)((nib <= 9 ? '0' : 'A') + (nib <= 9 ? nib : nib - 10));
                    printf((byte*)s, null);
                }
            }

            static TwoByteStr tbs;

            public struct TwoByteStr
            {
                public byte first;
                public byte second;
            }

        }

        public static void SetData(string name, object? data)
        {
            X.PrintLine("SetData");
            if (name == null)
                throw new ArgumentNullException(nameof(name));

            if (s_dataStore == null)
            {
                Interlocked.CompareExchange(ref s_dataStore, new Dictionary<string, object?>(), null);
            }

            lock (s_dataStore)
            {
                s_dataStore[name] = data;
            }
        }

#pragma warning disable CS0067 // events raised by the VM
        public static event UnhandledExceptionEventHandler? UnhandledException;

        public static event EventHandler<FirstChanceExceptionEventArgs>? FirstChanceException;
#pragma warning restore CS0067

        public static event EventHandler? ProcessExit;

        internal static void OnProcessExit()
        {
            AssemblyLoadContext.OnProcessExit();
            if (EventSource.IsSupported)
            {
                EventListener.DisposeOnShutdown();
            }

            ProcessExit?.Invoke(AppDomain.CurrentDomain, EventArgs.Empty);
        }

        /// <summary>
        /// Try to get the value of the switch.
        /// </summary>
        /// <param name="switchName">The name of the switch</param>
        /// <param name="isEnabled">A variable where to place the value of the switch</param>
        /// <returns>A return value of true represents that the switch was set and <paramref name="isEnabled"/> contains the value of the switch</returns>
        public static bool TryGetSwitch(string switchName, out bool isEnabled)
        {
            if (switchName == null)
                throw new ArgumentNullException(nameof(switchName));
            if (switchName.Length == 0)
                throw new ArgumentException(SR.Argument_EmptyName, nameof(switchName));

            if (s_switches != null)
            {
                lock (s_switches)
                {
                    if (s_switches.TryGetValue(switchName, out isEnabled))
                        return true;
                }
            }

            if (GetData(switchName) is string value && bool.TryParse(value, out isEnabled))
            {
                return true;
            }

            isEnabled = false;
            return false;
        }

        /// <summary>
        /// Assign a switch a value
        /// </summary>
        /// <param name="switchName">The name of the switch</param>
        /// <param name="isEnabled">The value to assign</param>
        public static void SetSwitch(string switchName, bool isEnabled)
        {
            if (switchName == null)
                throw new ArgumentNullException(nameof(switchName));
            if (switchName.Length == 0)
                throw new ArgumentException(SR.Argument_EmptyName, nameof(switchName));

            if (s_switches == null)
            {
                // Compatibility switches are rarely used. Initialize the Dictionary lazily
                Interlocked.CompareExchange(ref s_switches, new Dictionary<string, bool>(), null);
            }

            lock (s_switches)
            {
                s_switches[switchName] = isEnabled;
            }
        }

#if !CORERT
        internal static unsafe void Setup(char** pNames, char** pValues, int count)
        {
            Debug.Assert(s_dataStore == null, "s_dataStore is not expected to be inited before Setup is called");
            s_dataStore = new Dictionary<string, object?>(count);
            for (int i = 0; i < count; i++)
            {
                s_dataStore.Add(new string(pNames[i]), new string(pValues[i]));
            }
        }
#endif
    }
}
