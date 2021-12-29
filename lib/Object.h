#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include "Primitives.h"

#define DefineMethod(objectType, methodName, argCount)                                             \
    do                                                                                             \
    {                                                                                              \
        this->methodName = Object__Trampoline((Object)this, objectType##__##methodName, argCount); \
    } while (0);

#define OBJECT_METHOD_COUNT 2
#define OBJECT_BODY             \
    size_t codePageSize;        \
    unsigned char *codePage;    \
    unsigned char *codePagePtr; \
    void (*Finalize)(void);     \
    int64_t (*GetHashCode)(void);

typedef struct object_t
{
    OBJECT_BODY
} object_t, *Object;

pointer Object__Trampoline(Object this, pointer target, int argCount);
pointer Object__New(size_t size, int methodCount);
void Object__Prepare(Object this);
void Object__DefineMethods(Object this);
void Object__Finalize(Object this);

#endif
