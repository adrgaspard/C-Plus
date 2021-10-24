#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Object.h"
#include "StringManager.h"

#define EXCEPTION(message) Exception_New(__FILE__, __LINE__, message)->raise();

#define MEMORY_ALLOCATION_EXCEPTION "The system failed to allocate memory."
#define NOT_SUPPORTED_EXCEPTION "An unsupported case has occured."
#define ARGUMENT_EXCEPTION "A given argument is wrong."

#define EXCEPTION_METHOD_COUNT OBJECT_METHOD_COUNT + 2
#define EXCEPTION_BODY \
    OBJECT_BODY \
    \
    StringManager* messageContainer; \
    StringManager* fileNameContainer; \
    int lineNumber; \
    string (*getMessage)(void); \
    void (*raise)(void);

typedef struct Exception {
    EXCEPTION_BODY
} Exception;

Exception* Exception_New(const string fileName, const int lineNumber, const string message);

#endif
