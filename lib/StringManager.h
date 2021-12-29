#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include "Object.h"

#define STRINGMANAGER_METHOD_COUNT OBJECT_METHOD_COUNT + 6

#define STRINGMANAGER_BODY                                   \
    OBJECT_BODY                                              \
                                                             \
    string _value;                                           \
                                                             \
    string (*GetValue)(void);                                \
    struct stringManager_t *(*SetValue)(const string value); \
    bool (*EqualsTo)(const string other);                    \
    int (*CompareTo)(const string other);                    \
    int (*GetLength)(void);                                  \
    struct stringManager_t *(*Append)(const string toAppend);

typedef struct stringManager_t
{
    STRINGMANAGER_BODY
} stringManager_t, *StringManager;

StringManager StringManager__New(const string value);

#endif