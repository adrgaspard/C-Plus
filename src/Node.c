#include <malloc.h>

#include "../lib/C+/Node.h"
#include "../lib/C+/Exception.h"

#pragma region INode

void INode_finalize(FinalizeOptions options, INode* this)
{
    switch (options)
    {
    case RecursiveContentFinalization:
        this->content->finalize(options);
        break;
    case ContentFinalization:
        this->content->finalize(WeakFinalization);
        break;
    case WeakFinalization:
        break;
    default:
        EXCEPTION(NOT_SUPPORTED_EXCEPTION);
        break;
    }
    this->base.finalize();
}

Object* INode_getContent(INode* this)
{
    return this->content;
}

void INode_DefineMethods(INode *this)
{
    Object_DefineMethods((Object*)this);
    METHOD(INode, finalize, 1);
    METHOD(INode, getContent, 0);
}

#pragma endregion

#pragma region IArrayNode

void IArrayNode_finalize(FinalizeOptions options, IArrayNode* this)
{
    int i;
    switch (options)
    {
    case RecursiveContentFinalization:
        for(i = 0; i < this->getActualSize(); i++)
        {
            this->getContent(i)->finalize(options);
        }  
        break;
    case ContentFinalization:
        for (i = 0; i < this->getActualSize(); i++)
        {
            this->getContent(i)->finalize(WeakFinalization);
        }
        break;
    case WeakFinalization:
        break;
    default:
        EXCEPTION(NOT_SUPPORTED_EXCEPTION);
        break;
    }
    free(this->content);
    this->base.finalize();
}

Object* IArrayNode_getContent(int index, IArrayNode* this)
{
    if (index < 0 && index >= this->getActualSize())
    {
        EXCEPTION(ARGUMENT_EXCEPTION);
    }
    return this->content[index];
}

int IArrayNode_getActualSize(IArrayNode* this)
{
    return this->actualSize;
}

int IArrayNode_getMaxSize(IArrayNode* this)
{
    return this->maxSize;
}

bool IArrayNode_add(Object* item, IArrayNode* this)
{
    if (this->getActualSize() >= this->getMaxSize())
    {
        return false;
    }
    this->content[this->getActualSize()] = item;
    this->actualSize = this->getActualSize() + 1;
    return true;
}

bool IArrayNode_remove(Object* item, FinalizeOptions options, IArrayNode* this)
{
    int i;
    for(i = 0; i < this->getActualSize(); i++)
    {
        if (this->getContent(i) == item)
        {
            return this->removeByIndex(i, options);
        }
    }
    return false;
}

bool IArrayNode_removeByIndex(int index, FinalizeOptions options, IArrayNode* this)
{
    int i;
    if (index < 0 || index >= this->getActualSize())
    {
        return false;
    }
    if (options & RecursiveContentFinalization == RecursiveContentFinalization)
    {
        this->content[index]->finalize(options);
    }
    else if (options & ContentFinalization == ContentFinalization)
    {
        this->content[index]->finalize(WeakFinalization);
    }
    for (i = index; i < this->getActualSize() - 1; i++)
    {
        this->content[i] = this->content[i + 1];
    }
    this->content[this->getActualSize()] = NULL;
    this->actualSize = this->getActualSize() - 1;
    return true;
}

void IArrayNode_DefineMethods(IArrayNode *this)
{
    Object_DefineMethods((Object*)this);
    METHOD(IArrayNode, finalize, 1);
    METHOD(IArrayNode, getContent, 1);
    METHOD(IArrayNode, getActualSize, 0);
    METHOD(IArrayNode, getMaxSize, 0);
    METHOD(IArrayNode, add, 1);
    METHOD(IArrayNode, remove, 2);
    METHOD(IArrayNode, removeByIndex, 2);
}

#pragma endregion

#pragma region LinkedNode

LinkedNode* LinkedNode_getNext(LinkedNode* this)
{
    return this->next;
}

LinkedNode* LinkedNode_setNext(LinkedNode* next, LinkedNode* this)
{
    this->next = next;
    return this;
}

void LinkedNode_DefineMethods(LinkedNode* this)
{
    INode_DefineMethods((INode*)this);
    //METHOD(LinkedNode, finalize, 1);
    //METHOD(LinkedNode, getContent, 0);
    METHOD(LinkedNode, getNext, 0);
    METHOD(LinkedNode, setNext, 1);
}

LinkedNode* LinkedNode_New(Object* content)
{
    LinkedNode* this = Base_New(sizeof(LinkedNode), LINKEDNODE_METHOD_COUNT);
    LinkedNode_DefineMethods(this);
    Base_Prepare(&this->base);

    this->content = content;
    this->setNext(NULL);

    return this;
}

#pragma endregion

#pragma region DoubleLinkedNode

DoubleLinkedNode* DoubleLinkedNode_getPrev(DoubleLinkedNode* this)
{
    return this->prev;
}

DoubleLinkedNode* DoubleLinkedNode_setPrev(DoubleLinkedNode* prev, DoubleLinkedNode* this)
{
    this->prev = prev;
    return this;
}

DoubleLinkedNode* DoubleLinkedNode_getNext(DoubleLinkedNode* this)
{
    return this->next;
}

DoubleLinkedNode* DoubleLinkedNode_setNext(DoubleLinkedNode* next, DoubleLinkedNode* this)
{
    this->next = next;
    return this;
}

void DoubleLinkedNode_DefineMethods(DoubleLinkedNode* this)
{
    INode_DefineMethods((INode*)this);
    //METHOD(DoubleLinkedNode, finalize, 1);
    //METHOD(DoubleLinkedNode, getContent, 0);
    METHOD(DoubleLinkedNode, getPrev, 0);
    METHOD(DoubleLinkedNode, setPrev, 1);
    METHOD(DoubleLinkedNode, getNext, 0);
    METHOD(DoubleLinkedNode, setNext, 1);
}

DoubleLinkedNode* DoubleLinkedNode_New(Object* content)
{
    DoubleLinkedNode* this = Base_New(sizeof(DoubleLinkedNode), DOUBLELINKEDNODE_METHOD_COUNT);
    DoubleLinkedNode_DefineMethods(this);
    Base_Prepare(&this->base);

    this->content = content;
    this->setPrev(NULL);
    this->setNext(NULL);

    return this;
}

#pragma endregion

#pragma region TreeNode

Object* TreeNode_getContent(TreeNode* this)
{
    return this->content;
}

TreeNode* TreeNode_getParent(TreeNode* this)
{
    return this->parent;
}

TreeNode* TreeNode_setParent(TreeNode* parent, TreeNode* this)
{
    this->parent = parent;
    return this;
}

TreeNode* TreeNode_getLeftChild(TreeNode* this)
{
    return this->leftChild;
}

TreeNode* TreeNode_setLeftChild(TreeNode* leftChild, TreeNode* this)
{
    this->leftChild = leftChild;
    return this;
}

TreeNode* TreeNode_getRightChild(TreeNode* this)
{
    return this->rightChild;
}

TreeNode* TreeNode_setRightChild(TreeNode* rightChild, TreeNode* this)
{
    this->rightChild = rightChild;
    return this;
}

void TreeNode_DefineMethods(TreeNode* this)
{
    INode_DefineMethods((INode*)this);
    //METHOD(TreeNode, finalize, 1);
    //METHOD(TreeNode, getContent, 0);
    METHOD(TreeNode, getParent, 0);
    METHOD(TreeNode, setParent, 1);
    METHOD(TreeNode, getLeftChild, 0);
    METHOD(TreeNode, setLeftChild, 1);
    METHOD(TreeNode, getRightChild, 0);
    METHOD(TreeNode, setRightChild, 1);
}

TreeNode* TreeNode_New(Object* content)
{
    TreeNode* this = Base_New(sizeof(TreeNode), TREENODE_METHOD_COUNT);
    TreeNode_DefineMethods(this);
    Base_Prepare(&this->base);

    this->content = content;
    this->setParent(NULL);
    this->setLeftChild(NULL);
    this->setRightChild(NULL);

    return this;
}

#pragma endregion

#pragma region LinkedArrayNode

LinkedArrayNode* LinkedArrayNode_getNext(LinkedArrayNode* this)
{
    return this->next;
}

LinkedArrayNode* LinkedArrayNode_setNext(LinkedArrayNode* next, LinkedArrayNode* this)
{
    this->next = next;
    return this;
}

void LinkedArrayNode_DefineMethods(LinkedArrayNode* this)
{
    IArrayNode_DefineMethods((IArrayNode*)this);
    //METHOD(LinkedArrayNode, finalize, 1);
    //METHOD(LinkedArrayNode, getContent, 1);
    //METHOD(LinkedArrayNode, getActualSize, 0);
    //METHOD(LinkedArrayNode, getMaxSize, 0);
    METHOD(LinkedArrayNode, getNext, 0);
    METHOD(LinkedArrayNode, setNext, 1);
    //METHOD(LinkedArrayNode, add, 1);
    //METHOD(LinkedArrayNode, remove, 2);
    //METHOD(LinkedArrayNode, removeByIndex, 2);
}

LinkedArrayNode* LinkedArrayNode_New(int maxSize)
{
    LinkedArrayNode* this = Base_New(sizeof(LinkedArrayNode), LINKEDARRAYNODE_METHOD_COUNT);
    LinkedArrayNode_DefineMethods(this);
    Base_Prepare(&this->base);

    this->content = (Object**)calloc(maxSize, sizeof(Object*));
    if (!this->content)
    {
        EXCEPTION(MEMORY_ALLOCATION_EXCEPTION);
    }
    this->actualSize = 0;
    this->maxSize = maxSize;
    this->setNext(NULL);

    return this;
}

#pragma endregion

#pragma region DoubleLinkedArrayNode

DoubleLinkedArrayNode* DoubleLinkedArrayNode_getPrev(DoubleLinkedArrayNode* this)
{
    return this->prev;
}

DoubleLinkedArrayNode* DoubleLinkedArrayNode_setPrev(DoubleLinkedArrayNode* prev, DoubleLinkedArrayNode* this)
{
    this->prev = prev;
    return this;
}

DoubleLinkedArrayNode* DoubleLinkedArrayNode_getNext(DoubleLinkedArrayNode* this)
{
    return this->next;
}

DoubleLinkedArrayNode* DoubleLinkedArrayNode_setNext(DoubleLinkedArrayNode* next, DoubleLinkedArrayNode* this)
{
    this->next = next;
    return this;
}

void DoubleLinkedArrayNode_DefineMethods(DoubleLinkedArrayNode* this)
{
    IArrayNode_DefineMethods((IArrayNode*)this);
    //METHOD(DoubleLinkedArrayNode, finalize, 1);
    //METHOD(DoubleLinkedArrayNode, getContent, 1);
    //METHOD(DoubleLinkedArrayNode, getActualSize, 0);
    //METHOD(DoubleLinkedArrayNode, getMaxSize, 0);
    METHOD(DoubleLinkedArrayNode, getPrev, 0);
    METHOD(DoubleLinkedArrayNode, setPrev, 1);
    METHOD(DoubleLinkedArrayNode, getNext, 0);
    METHOD(DoubleLinkedArrayNode, setNext, 1);
    //METHOD(DoubleLinkedArrayNode, add, 1);
    //METHOD(DoubleLinkedArrayNode, remove, 2);
    //METHOD(DoubleLinkedArrayNode, removeByIndex, 2);
}

DoubleLinkedArrayNode* DoubleLinkedArrayNode_New(int maxSize)
{
    DoubleLinkedArrayNode* this = Base_New(sizeof(DoubleLinkedArrayNode), DOUBLELINKEDARRAYNODE_METHOD_COUNT);
    DoubleLinkedArrayNode_DefineMethods(this);
    Base_Prepare(&this->base);

    this->content = (Object**)calloc(maxSize, sizeof(Object*));
    if (!this->content)
    {
        EXCEPTION(MEMORY_ALLOCATION_EXCEPTION);
    }
    this->actualSize = 0;
    this->maxSize = maxSize;
    this->setPrev(NULL);
    this->setNext(NULL);

    return this;
}

#pragma endregion
