#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

#include "../lib/C+/Object.h"

#pragma region Base

static const unsigned char trampoline[] = {
    // MOV RDI, 0x0
    0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // JMP [RIP + 0]
    0xff, 0x25, 0x00, 0x00, 0x00, 0x00,
    // DQ 0x0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void Base_Finalize(Base* this)
{
    int unmapResult = munmap(this->codePage, this->codePageSize);
    assert(!unmapResult);
    free(this);
}

void* Base_Trampoline(Base *this, void *target, int argCount)
{
    unsigned char opcode[][2] = {
        0x48, 0xbf,
        0x48, 0xbe,
        0x48, 0xba,
        0x48, 0xb9,
        0x49, 0xb8,
        0x49, 0xb9,
    };
    memcpy(this->codePagePtr, opcode[argCount], sizeof(opcode[argCount]));
    memcpy(this->codePagePtr + 2, &this, sizeof(void *));
    memcpy(this->codePagePtr + 10, trampoline + 10, sizeof(trampoline) - 10);
    memcpy(this->codePagePtr + 16, &target, sizeof(void *));
    this->codePagePtr += sizeof(trampoline);
    return this->codePagePtr - sizeof(trampoline);
}

void* Base_New(size_t size, int methodCount)
{
    methodCount += 1;
    Base *this = (Base*)calloc(1, size);
    this->codePageSize = methodCount * sizeof(trampoline);
    this->codePageSize = 4096;
    this->codePage = mmap(NULL, this->codePageSize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    this->codePagePtr = this->codePage;
    this->finalize = Base_Trampoline(this, Base_Finalize, 0);
    return this;
}

void Base_Prepare(Base *this)
{
    int protectResult = mprotect(this->codePage, this->codePageSize, PROT_READ | PROT_EXEC);
    assert(!protectResult);
}

#pragma endregion

#pragma region Object

void Object_finalize(FinalizeOptions options, Object* this)
{
    this->base.finalize();
}

long Object_getHashCode(Object* this)
{
    return (long)(intptr_t)(void*)(&this->base);
}

void Object_DefineMethods(Object* this)
{
    METHOD(Object, finalize, 1);
    METHOD(Object, getHashCode, 0);
}

#pragma endregion
