#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include "Object.h"
#include "Primitive.h"

#define STRINGMANAGER_METHOD_COUNT OBJECT_METHOD_COUNT + 6
#define STRINGMANAGER_BODY \
    OBJECT_BODY \
    \
    string value; \
    string (*getValue)(void); \
    struct StringManager* (*setValue)(const string value); \
    \
    bool (*equalsTo)(const string other); \
    int (*compareTo)(const string other); \
    int (*getLength)(void); \
    \
    struct StringManager* (*append)(const string toAppend);

typedef struct StringManager {
    STRINGMANAGER_BODY
} StringManager;

StringManager* StringManager_New(const string value);

#endif
