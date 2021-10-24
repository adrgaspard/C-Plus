#include <stdio.h>
#include <stdlib.h>

#include "../lib/C+/Exception.h"

void Exception_finalize(FinalizeOptions options, Exception* this)
{
    this->messageContainer->finalize(options);
    this->fileNameContainer->finalize(options);
    this->base.finalize();
}

string Exception_getMessage(Exception* this)
{
    return this->messageContainer->getValue();
}

void Exception_raise(Exception* this)
{
    printf("%s:%d > Exception raised : %s\n", this->fileNameContainer->getValue(), this->lineNumber, this->messageContainer->getValue());
    exit(1);
}

void Exception_DefineMethods(Exception* this)
{
    Object_DefineMethods((Object*)this);
    METHOD(Exception, finalize, 1)
    METHOD(Exception, getMessage, 0);
    METHOD(Exception, raise, 0);
}

Exception* Exception_New(const string fileName, const int lineNumber, const string message)
{
    Exception* this = (Exception*)Base_New(sizeof(Exception), EXCEPTION_METHOD_COUNT);
    Exception_DefineMethods(this);
    Base_Prepare(&this->base);

    this->messageContainer = StringManager_New(message);
    this->fileNameContainer = StringManager_New(fileName);
    this->lineNumber = lineNumber;

    return this;
}
