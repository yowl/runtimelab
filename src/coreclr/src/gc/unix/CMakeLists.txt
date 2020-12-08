set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories("../env")

include(configure.cmake)

set(GC_PAL_SOURCES
    events.cpp
    cgroup.cpp)

if(CLR_CMAKE_HOST_UNIX)
    set ( GC_PAL_SOURCES
    ${GC_PAL_SOURCES}
    gcenv.unix.cpp)
endif(CLR_CMAKE_HOST_UNIX)

if(CLR_CMAKE_TARGET_ARCH_WASM)
    set ( GC_PAL_SOURCES
    ${GC_PAL_SOURCES}
    gcenv.wasm.cpp)
endif(CLR_CMAKE_TARGET_ARCH_WASM)

add_library(gc_unix STATIC ${GC_PAL_SOURCES} ${VERSION_FILE_PATH})
