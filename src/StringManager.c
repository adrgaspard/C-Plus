#include <string.h>
#include <malloc.h>
#include <stdlib.h> // TODO REMOVE

#include "../lib/StringManager.h"

void StringManager__Finalize(StringManager this)
{
    this->SetValue(NULL);
    Object__Finalize((Object)this);
}

string StringManager__GetValue(StringManager this)
{
    return this->_value;
}

StringManager StringManager__SetValue(const string value, StringManager this)
{
    if (this->GetValue() != NULL)
    {
        free(this->_value);
    }
    if (value == NULL)
    {
        this->_value = NULL;
    }
    else
    {
        this->_value = (string)calloc(strlen(value) + 1, sizeof(char));
        if (!this->_value)
        {
            //EXCEPTION(MEMORY_ALLOCATION_EXCEPTION); TODO
            exit(1);
        }
        strcpy(this->_value, value);
    }
    return this;
}

bool StringManager__EqualsTo(const string other, StringManager this)
{
    return this->CompareTo(other) == 0;
}

int StringManager__CompareTo(const string other, StringManager this)
{
    return strcmp(this->GetValue(), other);
}

int StringManager__GetLength(StringManager this)
{
    if (this->GetValue() == NULL)
    {
        return 0;
    }
    return strlen(this->GetValue());
}

StringManager StringManager__Append(const string toAppend, StringManager this)
{
    if (toAppend == NULL || strcmp(toAppend, "") == 0)
    {
        return this;
    }
    if (this->GetValue() == NULL || this->EqualsTo(""))
    {
        this->SetValue(toAppend);
    }
    this->_value = (string)realloc(this->_value, (strlen(this->_value) + strlen(toAppend) + 1) * sizeof(char));
    if (!this->_value)
    {
        //EXCEPTION(MEMORY_ALLOCATION_EXCEPTION); TODO
        exit(1);
    }
    strcat(this->_value, toAppend);
    return this;
}

void StringManager__DefineMethods(StringManager this)
{
    // Call super class method
    Object__DefineMethods((Object)this);

    // StringManager new methods
    DefineMethod(StringManager, Finalize, 0);
    DefineMethod(StringManager, GetValue, 0);
    DefineMethod(StringManager, SetValue, 1);
    DefineMethod(StringManager, EqualsTo, 1);
    DefineMethod(StringManager, CompareTo, 1);
    DefineMethod(StringManager, GetLength, 0);
    DefineMethod(StringManager, Append, 1);
}

StringManager StringManager__New(const string value)
{
    StringManager this = (StringManager)Object__New(sizeof(stringManager_t), STRINGMANAGER_METHOD_COUNT);
    StringManager__DefineMethods(this);
    Object__Prepare((Object)this);
    this->SetValue(value);
    return this;
}
