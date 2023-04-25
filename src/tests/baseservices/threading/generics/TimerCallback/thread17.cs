// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
using System;
using System.Threading;
using Xunit;

interface IGen
{
	void Target<U>(object p);
}

struct Gen : IGen
{
	public void Target<U>(object p)
	{		
		//dummy line to avoid warnings
		Test_thread17.Eval(typeof(U)!=p.GetType());
		if (Test_thread17.Xcounter>=Test_thread17.nThreads)
		{
			ManualResetEvent evt = (ManualResetEvent) p;	
			evt.Set();
		}
		else
		{
			Interlocked.Increment(ref Test_thread17.Xcounter);	
		}
	}
	
	public static void ThreadPoolTest<U>()
	{
		ManualResetEvent evt = new ManualResetEvent(false);		
		
		IGen obj = new Gen();

		TimerCallback tcb = new TimerCallback(obj.Target<U>);
		Timer timer = new Timer(tcb,evt,Test_thread17.delay,Test_thread17.period);
	
		evt.WaitOne();
		timer.Dispose();
		Test_thread17.Eval(Test_thread17.Xcounter>=Test_thread17.nThreads);
		Test_thread17.Xcounter = 0;
	}
}

public class Test_thread17
{
	public static int delay = 0;
	public static int period = 2;
	public static int nThreads = 5;
	public static int counter = 0;
	public static int Xcounter = 0;
	public static bool result = true;
	public static void Eval(bool exp)
	{
		counter++;
		if (!exp)
		{
			result = exp;
			Console.WriteLine("Test Failed at location: " + counter);
		}
	
	}
	
	[Fact]
	public static int TestEntryPoint()
	{
		Gen.ThreadPoolTest<object>();
		Gen.ThreadPoolTest<string>();
		Gen.ThreadPoolTest<Guid>();
		Gen.ThreadPoolTest<int>(); 
		Gen.ThreadPoolTest<double>(); 

		if (result)
		{
			Console.WriteLine("Test Passed");
			return 100;
		}
		else
		{
			Console.WriteLine("Test Failed");
			return 1;
		}
	}
}		


