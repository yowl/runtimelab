using System;
using System.Runtime.InteropServices;

namespace HelloWorld
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }

        [UnmanagedCallersOnly(EntryPoint = "emscripten_get_callstack")]
        public static void emscripten_get_callstack(double width, double height)
        {
        }
    }
}
