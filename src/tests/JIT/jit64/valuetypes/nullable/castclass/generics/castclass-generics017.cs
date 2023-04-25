// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.


using System.Runtime.InteropServices;
using System;
using Xunit;

public class NullableTest
{
    private static bool BoxUnboxToNQ<T>(T o)
    {
        return Helper.Compare((GCHandle)(ValueType)(object)o, Helper.Create(default(GCHandle)));
    }

    private static bool BoxUnboxToQ<T>(T o)
    {
        return Helper.Compare((GCHandle?)(ValueType)(object)o, Helper.Create(default(GCHandle)));
    }

    [Fact]
    public static int TestEntryPoint()
    {
        GCHandle? s = Helper.Create(default(GCHandle));

        if (BoxUnboxToNQ(s) && BoxUnboxToQ(s))
            return ExitCode.Passed;
        else
            return ExitCode.Failed;
    }
}


