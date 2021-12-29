#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

#include "../lib/Object.h"

static const unsigned char trampoline[24] = {
    // MOV RDI, 0x0
    0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // JMP [RIP + 0]
    0xff, 0x25, 0x00, 0x00, 0x00, 0x00,
    // DQ 0x0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void Object__Finalize(Object this)
{
    int unmapResult = munmap(this->codePage, this->codePageSize);
    assert(!unmapResult);
    free(this);
}

pointer Object__Trampoline(Object this, pointer target, int argCount)
{
    unsigned char opcode[6][2] = {
        { 0x48, 0xbf },
        { 0x48, 0xbe },
        { 0x48, 0xba },
        { 0x48, 0xb9 },
        { 0x49, 0xb8 },
        { 0x49, 0xb9 }
    };
    memcpy(this->codePagePtr, opcode[argCount], sizeof(opcode[argCount]));
    memcpy(this->codePagePtr + 2, &this, sizeof(pointer));
    memcpy(this->codePagePtr + 10, trampoline + 10, sizeof(trampoline) - 10);
    memcpy(this->codePagePtr + 16, &target, sizeof(pointer));
    this->codePagePtr += sizeof(trampoline);
    return this->codePagePtr - sizeof(trampoline);
}

pointer Object__New(size_t size, int methodCount)
{
    methodCount += 1;
    Object this = (Object)calloc(1, size);
    this->codePageSize = methodCount * sizeof(trampoline);
    this->codePageSize = 4096;
    this->codePage = mmap(NULL, this->codePageSize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    this->codePagePtr = this->codePage;
    this->Finalize = Object__Trampoline(this, Object__Finalize, 0);
    return this;
}

void Object__Prepare(Object this)
{
    int protectResult = mprotect(this->codePage, this->codePageSize, PROT_READ | PROT_EXEC);
    assert(!protectResult);
}

int64_t Object__GetHashCode(Object this)
{
    return (int64_t)(intptr_t)(pointer)(this);
}

void Object__DefineMethods(Object this)
{
    DEFINE_METHOD(Object, Finalize, 0);
    DEFINE_METHOD(Object, GetHashCode, 0);
}
