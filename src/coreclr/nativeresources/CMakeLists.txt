project(nativeresourcestring)

if (CLR_CMAKE_BUILD_LLVM_JIT)
    add_definitions(-DPAL_STDCPP_COMPAT)
    include_directories("../pal/src/include")
    include_directories("/home/scott/github/runtimelab/artifacts/obj/coreclr/Linux.x64.Debug/pal/src")
endif(CLR_CMAKE_BUILD_LLVM_JIT)

add_library_clr(nativeresourcestring
  STATIC
  resourcestring.cpp
)

install_clr (TARGETS nativeresourcestring DESTINATIONS lib)
