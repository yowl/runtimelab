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
        public static int emscripten_get_callstack(int flags, IntPtr outBuf, int maxBytes)
        {
            throw new NotImplementedException();
        }

	[UnmanagedCallersOnly(EntryPoint = "dotnet_browser_entropy")]
        public static int dotnet_browser_entropy(IntPtr buffer, int bufferLength)
        {
            throw new NotImplementedException();
        }

	[UnmanagedCallersOnly(EntryPoint = "pthread_condattr_destroy")]
        public static int pthread_condattr_destroy(IntPtr attrs)
        {
            return 0;
        }

	[UnmanagedCallersOnly(EntryPoint = "signal")]
        public static IntPtr signal(int signum, IntPtr handler)
        {
            throw new NotImplementedException();
        }

	[UnmanagedCallersOnly(EntryPoint = "__cxa_thread_atexit")]
        public static int __cxa_thread_atexit(IntPtr p1, IntPtr p2, IntPtr p3)
        {
            return 0;
        }

        [UnmanagedCallersOnly(EntryPoint = "pthread_condattr_init")]
        public static int pthread_condattr_init(IntPtr attr)
        {
            return 0;
        }

	[UnmanagedCallersOnly(EntryPoint = "pthread_attr_init")]
        public static int pthread_attr_init(IntPtr attr)
        {
            return 0;
        }

        [UnmanagedCallersOnly(EntryPoint = "pthread_condattr_setclock")]
        public static int pthread_condattr_setclock(IntPtr attr, int p2)
        {
            return 0;
        }

        [UnmanagedCallersOnly(EntryPoint = "pthread_attr_setdetachstate")]
        public static int pthread_attr_setdetachstate(IntPtr attr, int p2)
        {
            return 0;
        }

        [UnmanagedCallersOnly(EntryPoint = "pthread_create")]
        public static int pthread_create(IntPtr tid, IntPtr p2, IntPtr start, IntPtr p4)
        {
            return 0;
        }

        [UnmanagedCallersOnly(EntryPoint = "pthread_attr_destroy")]
        public static int pthread_attr_destroy(IntPtr tid)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "pthread_getattr_np")]
        public static int pthread_getattr_np(IntPtr tid, IntPtr attr)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "pthread_attr_getstack")]
        public static int pthread_attr_getstack(IntPtr tid, IntPtr attr, IntPtr size)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "gai_strerror")]
        public static IntPtr gai_strerror(IntPtr error)
        {
            return IntPtr.Zero;
        }
        [UnmanagedCallersOnly(EntryPoint = "flock")]
        public static int flock(IntPtr fd, int op)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "pthread_setcancelstate")]
        public static int pthread_setcancelstate(int state, IntPtr statePtr)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "pthread_attr_setstacksize")]
        public static int pthread_attr_setstacksize(IntPtr attr, int size)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_getdents64")]
        public static int __sys_getdents64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_fadvise64_64")]
        public static int __sys_fadvise64_64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_prlimit64")]
        public static int __sys_prlimit64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_ugetrlimit")]
        public static int __sys_ugetrlimit(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_getrusage")]
        public static int __sys_getrusage(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_madvise1")]
        public static int __sys_madvise1(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_mlock")]
        public static int __sys_mlock(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_mprotect")]
        public static int __sys_mprotect(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_munlock")]
        public static int __sys_munlock(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_fstat64")]
        public static int __sys_fstat64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_stat64")]
        public static int __sys_stat64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_lstat64")]
        public static int __sys_lstat64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_ftruncate64")]
        public static int __sys_ftruncate64(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_getcwd")]
        public static int __sys_getcwd(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_getpid")]
        public static int __sys_getpid(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_read")]
        public static int __sys_read(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_readlink")]
        public static int __sys_readlink(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_unlink")]
        public static int __sys_unlink(int fd)
        {
            return 0;
        }
        [UnmanagedCallersOnly(EntryPoint = "__sys_socketcall")]
        public static int __sys_socketcall(int fd)
        {
            return 0;
        }
    }
}
