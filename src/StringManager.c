#include <string.h>
#include <malloc.h>

#include "../lib/C+/StringManager.h"
#include "../lib/C+/Exception.h"

void StringManager_finalize(FinalizeOptions options, StringManager* this)
{
    this->setValue(NULL);
    this->base.finalize();
}

string StringManager_getValue(StringManager* this)
{
    return this->value;
}

StringManager* StringManager_setValue(const string value, StringManager* this)
{
    if (this->getValue() != NULL)
    {
        free(this->value);
    }
    if (value == NULL)
    {
        this->value = NULL;
    }
    else
    {
        this->value = (string)calloc(strlen(value) + 1, sizeof(char));
        if (!this->value)
        {
            EXCEPTION(MEMORY_ALLOCATION_EXCEPTION);
        }
        strcpy(this->value, value);
    }
    return this;
}

bool StringManager_equalsTo(const string other, StringManager* this)
{
    return this->compareTo(other) == 0;
}

int StringManager_compareTo(const string other, StringManager* this)
{
    return strcmp(this->getValue(), other);
}

int StringManager_getLength(StringManager* this)
{
    if (this->getValue() == NULL)
    {
        return 0;
    }
    return strlen(this->getValue());
}

StringManager* StringManager_append(const string toAppend, StringManager* this)
{
    if (toAppend == NULL || strcmp(toAppend, "") == 0)
    {
        return this;
    }
    if (this->getValue() == NULL || this->equalsTo(""))
    {
        this->setValue(toAppend);
    }
    this->value = (string)realloc(this->value, (strlen(this->value) + strlen(toAppend) + 1) * sizeof(char));
    if (!this->value)
    {
        EXCEPTION(MEMORY_ALLOCATION_EXCEPTION);
    }
    strcat(this->value, toAppend);
    return this;
}

void StringManager_DefineMethods(StringManager* this)
{
    Object_DefineMethods((Object*)this);
    METHOD(StringManager, finalize, 1);
    METHOD(StringManager, getValue, 0);
    METHOD(StringManager, setValue, 1);
    METHOD(StringManager, equalsTo, 1);
    METHOD(StringManager, compareTo, 1);
    METHOD(StringManager, getLength, 0);
    METHOD(StringManager, append, 1);
}

StringManager* StringManager_New(const string value)
{

    StringManager* this = (StringManager*)Base_New(sizeof(StringManager), STRINGMANAGER_METHOD_COUNT);
    StringManager_DefineMethods(this);
    Base_Prepare(&this->base);

    this->setValue(value);

    return this;
}
