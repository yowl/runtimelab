if (CLR_CMAKE_BUILD_LLVM_JIT)
    add_definitions(-DPAL_STDCPP_COMPAT)
    include_directories("../pal/src/include")
    message("include bindir")
    include_directories(${CMAKE_CURRENT_BINARY_DIR}) # config.h
    include_directories("/home/scott/github/runtimelab/artifacts/obj/coreclr/Linux.x64.Debug/pal/src")
endif(CLR_CMAKE_BUILD_LLVM_JIT)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PALRT_SOURCES
    bstr.cpp
    coguid.cpp
    comem.cpp
    guid.cpp
    memorystream.cpp
    path.cpp
    variant.cpp
)

add_library_clr(palrt
    STATIC
    ${PALRT_SOURCES}
)

# Install the static PAL library for VS
install_clr(TARGETS palrt DESTINATIONS lib)
