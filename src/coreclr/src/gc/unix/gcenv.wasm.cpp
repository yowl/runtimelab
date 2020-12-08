// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include "common.h"
#include "gcenv.h"

// Emscripten does not provide a complete implementation of mmap and munmap: munmap cannot unmap partial allocations
// Emscripten does provide an implementation of posix_memalign which is used here.

struct _reserved
{
    void * address;
    size_t size;
    int used;
    int committed;
};

typedef struct _reserved reserved;

static int nextPos = 0;
static reserved blocks[1000];
static int init = 0;

static void Init()
{
    int i;
    for(i = 0; i < 1000; i++)
    {
        blocks[i].used = 0;
        blocks[i].committed = 0;
    }
}

static int FirstUnused()
{
    int i;
    for(i = 0; i < 1000; i++)
    {
        if(blocks[i].used == 0) return i;
    }
    assert(false);
}

static int FindBlock(void * address)
{
    int i;
    for(i = 0; i < 1000; i++)
    {
        if(blocks[i].used == 1 && blocks[i].address <= address && 
                (size_t)blocks[i].address + blocks[i].size > (size_t)address) return i;
    }
    printf("did not find block for address %p\n", address);
    return -1;
}

// Reserve virtual memory range.
// Parameters:
//  size      - size of the virtual memory range
//  alignment - requested memory alignment, 0 means no specific alignment requested
//  flags     - flags to control special settings like write watching
// Return:
//  Starting virtual address of the reserved range
static void* VirtualReserveInner(size_t size, size_t alignment, uint32_t flags)
{
    printf("VirtualReserveInner requested size %ld with alignment %lx OS_PAGE_SIZE %lx\n", size, alignment, OS_PAGE_SIZE);

    int b;
    b = FirstUnused();
    if(b == -1)
    {
        printf("no more spare blocks\n");
        assert(false);
    }

    assert(!(flags & VirtualReserveFlags::WriteWatch) && "WriteWatch not supported on Wasm");
    if (alignment < OS_PAGE_SIZE)
    {
        alignment = OS_PAGE_SIZE;
    }

    void * pRetVal;
    printf("VirtualReserveInner alignment is %zx\n", alignment);
    int result = posix_memalign(&pRetVal, alignment, size);
    if(result != 0)
    {
        printf("reserveinner failed\n");
        return NULL; // failed
    }
    memset(pRetVal, 0, size);
    printf("reserverinner %p %zd\n", pRetVal, size);
    blocks[b].address = pRetVal;
    blocks[b].size = size;
    blocks[b].used = 1;
    return pRetVal;
}

// Reserve virtual memory range.
// Parameters:
//  size      - size of the virtual memory range
//  alignment - requested memory alignment, 0 means no specific alignment requested
//  flags     - flags to control special settings like write watching
//  node      - the NUMA node to reserve memory on
// Return:
//  Starting virtual address of the reserved range
void* GCToOSInterface::VirtualReserve(size_t size, size_t alignment, uint32_t flags, uint16_t node)
{
    return VirtualReserveInner(size, alignment, flags);
}

// Release virtual memory range previously reserved using VirtualReserve
// Parameters:
//  address - starting virtual address
//  size    - size of the virtual memory range - ignored: emscripten does not support partial unmapping
// Return:
//  true if it has succeeded, false if it has failed
bool GCToOSInterface::VirtualRelease(void* address, size_t size)
{
    // WASM: TODO: if an attempt is made to release a partial range from an alloc, starting from the start of the range, this will release the whole range
    // This would cause corruption, but this case does not appear to happen at the time of writing
    printf("free %p %zx\n", address, size);
    int b;
    b = FindBlock(address);
    if(b == -1)
    {
        printf("releasing address not found %p\n", address);
        assert(false);
    }

    if(blocks[b].address != address)
    {
        printf("releasing address not same as reserved %p %p\n", address, blocks[b].address);
        assert(false);
    }
    if(blocks[b].size != size)
    {
        printf("releasing size not equal to reserved %zd %zd\n", size, blocks[b].size);
        //assert(false);
    }
    blocks[b].used = 0;
    free(address);

    return TRUE; // free() is void
}

// Commit virtual memory range.
// Parameters:
//  size      - size of the virtual memory range
// Return:
//  Starting virtual address of the committed range
void* GCToOSInterface::VirtualReserveAndCommitLargePages(size_t size, uint16_t node)
{
    // Wasm has no concept of large pages
    void* pRetVal = VirtualReserveInner(size, OS_PAGE_SIZE, 0);
    if (VirtualCommit(pRetVal, size, node))
    {
        return pRetVal;
    }

    return nullptr;
}

// Commit virtual memory range. For emscripten this is not implemented
// Parameters:
//  address - starting virtual address
//  size    - size of the virtual memory range
// Return:
//  true if it has succeeded, false if it has failed
bool GCToOSInterface::VirtualCommit(void* address, size_t size, uint16_t node)
{
    printf("VirtualCommit at %p size %zx\n", address, size);
    int b;
    b = FindBlock(address);
    if (b == -1)
    {
        printf("VirtualCommit address not found %p\n", address);
        assert(false);
    }
    //if (!blocks[b].committed)
    //{
    //    memset(address, 0, size);
    //    blocks[b].committed = TRUE;
    //}
    return TRUE;
}

// Decomit virtual memory range.  For emscripten this is not implemented
// Parameters:
//  address - starting virtual address
//  size    - size of the virtual memory range
// Return:
//  true if it has succeeded, false if it has failed
bool GCToOSInterface::VirtualDecommit(void* address, size_t size)
{
    printf("decommit at %p size %zx\n", address, size);
    int b;
    b = FindBlock(address);
    if(b == -1)
    {
        printf("decommit address not found %p\n", address);
        assert(false);
    }
    memset(address, 0, size);
    //blocks[b].committed = FALSE;
    return TRUE;
}

