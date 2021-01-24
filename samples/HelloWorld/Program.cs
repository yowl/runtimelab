using System;
using System.Runtime.InteropServices;

namespace HelloWorld
{
    class Program
    {
        static void Main(string[] args)
        {
PrintLine("start");
//            Console.WriteLine("Hello World!");
PrintLine("Hello World!");
PrintLine("end");
        }

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

    public static void PrintLine(string s)
    {
        PrintString(s);
        PrintString("\n");
    }

	    [DllImport("*")]
    private static unsafe extern int printf(byte* str, byte* unused);
    }

    public struct TwoByteStr
    {
      public byte first;
      public byte second;
    }
}
