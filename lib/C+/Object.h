#ifndef OBJECT_H
#define OBJECT_H

#define METHOD(objectType, methodName, argCount) \
    do { \
        this->methodName = Base_Trampoline(&this->base, objectType ## _ ## methodName, argCount); \
    } while (0);

#define OBJECT_METHOD_COUNT 2
#define OBJECT_BODY \
    Base base; \
    void (*finalize)(FinalizeOptions options); \
    long (*getHashCode)(void);

typedef enum FinalizeOptions {WeakFinalization = 0, ContentFinalization = 1, RecursiveContentFinalization = 3, StrongFinalization = 7} FinalizeOptions;

typedef struct Base {
    size_t codePageSize;
    unsigned char *codePage;
    unsigned char *codePagePtr;
    void (*finalize)(void);
} Base;

typedef struct Object {
    OBJECT_BODY
} Object;

void* Base_Trampoline(Base* this, void* target, int argCount);
void* Base_New(size_t size, int methodCount);
void Base_Prepare(Base* this);

void Object_DefineMethods(Object* this);

#endif
