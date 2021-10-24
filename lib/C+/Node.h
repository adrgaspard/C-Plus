#ifndef NODE_H
#define NODE_H

#include "Object.h"
#include "Primitive.h"

#define INODE_METHOD_COUNT OBJECT_METHOD_COUNT + 1
#define INODE_BODY \
    OBJECT_BODY \
    \
    Object* content; \
    Object* (*getContent)(void);

#define IARRAYNODE_METHOD_COUNT OBJECT_METHOD_COUNT + 6
#define IARRAYNODE_BODY \
    OBJECT_BODY \
    \
    Object** content; \
    Object* (*getContent)(int index); \
    int actualSize; \
    int (*getActualSize)(void); \
    int maxSize; \
    int (*getMaxSize)(void); \
    \
    bool (*add)(Object* item); \
    bool (*remove)(Object* item, FinalizeOptions options); \
    bool (*removeByIndex)(int index, FinalizeOptions options);

#define LINKEDNODE_METHOD_COUNT INODE_METHOD_COUNT + 2
#define LINKEDNODE_BODY \
    INODE_BODY \
    \
    struct LinkedNode* next; \
    struct LinkedNode* (*getNext)(void); \
    struct LinkedNode* (*setNext)(const struct LinkedNode* next);

#define DOUBLELINKEDNODE_METHOD_COUNT INODE_METHOD_COUNT + 4
#define DOUBLELINKEDNODE_BODY \
    INODE_BODY \
    \
    struct DoubleLinkedNode* prev; \
    struct DoubleLinkedNode* (*getPrev)(void); \
    struct DoubleLinkedNode* (*setPrev)(const struct DoubleLinkedNode* prev); \
    struct DoubleLinkedNode* next; \
    struct DoubleLinkedNode* (*getNext)(void); \
    struct DoubleLinkedNode* (*setNext)(const struct DoubleLinkedNode* next);

#define TREENODE_METHOD_COUNT INODE_METHOD_COUNT + 6
#define TREENODE_BODY \
    INODE_BODY \
    \
    struct TreeNode* parent; \
    struct TreeNode* (*getParent)(void); \
    struct TreeNode* (*setParent)(const struct TreeNode* parent); \
    struct TreeNode* leftChild; \
    struct TreeNode* (*getLeftChild)(void); \
    struct TreeNode* (*setLeftChild)(const struct TreeNode* leftChild); \
    struct TreeNode* rightChild; \
    struct TreeNode* (*getRightChild)(void); \
    struct TreeNode* (*setRightChild)(const struct TreeNode* rightChild);

#define LINKEDARRAYNODE_METHOD_COUNT IARRAYNODE_METHOD_COUNT + 2
#define LINKEDARRAYNODE_BODY \
    IARRAYNODE_BODY \
    \
    struct LinkedArrayNode* next; \
    struct LinkedArrayNode* (*getNext)(void); \
    struct LinkedArrayNode* (*setNext)(const struct LinkedArrayNode* next);

#define DOUBLELINKEDARRAYNODE_METHOD_COUNT IARRAYNODE_METHOD_COUNT + 4
#define DOUBLELINKEDARRAYNODE_BODY \
    IARRAYNODE_BODY \
    \
    struct DoubleLinkedArrayNode* prev; \
    struct DoubleLinkedArrayNode* (*getPrev)(void); \
    struct DoubleLinkedArrayNode* (*setPrev)(const struct DoubleLinkedArrayNode* prev); \
    struct DoubleLinkedArrayNode* next; \
    struct DoubleLinkedArrayNode* (*getNext)(void); \
    struct DoubleLinkedArrayNode* (*setNext)(const struct DoubleLinkedArrayNode* next);

typedef struct INode {
    INODE_BODY
} INode;

typedef struct IArrayNode {
    IARRAYNODE_BODY
} IArrayNode;

typedef struct LinkedNode {
    LINKEDNODE_BODY
} LinkedNode;

typedef struct DoubleLinkedNode {
    DOUBLELINKEDNODE_BODY
} DoubleLinkedNode;

typedef struct TreeNode {
    TREENODE_BODY
} TreeNode;

typedef struct LinkedArrayNode {
    LINKEDARRAYNODE_BODY    
} LinkedArrayNode;

typedef struct DoubleLinkedArrayNode {
    DOUBLELINKEDARRAYNODE_BODY
} DoubleLinkedArrayNode;

LinkedNode* LinkedNode_New(Object* content);
DoubleLinkedNode* DoubleLinkedNode_New(Object* content);
TreeNode* TreeNode_New(Object* content);
LinkedArrayNode* LinkedArrayNode_New(int maxSize);
DoubleLinkedArrayNode* DoubleLinkedArrayNode_New(int maxSize);

#endif
