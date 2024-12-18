#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define VALUES_FOR_ERROR __LINE__, __FUNCTION__, __FILE__

enum NODE_TYPE
{
    NUMBER         = 1,
    VARIABLE       = 2,
    OPERATION      = 3,
    TRIG_OPERATION = 4,
    CONST          = 5,
    EMPTY          = 0
};

enum NODE_OPERATIONS
{
    ADD         = 1,
    SUB         = 2,
    MUL         = 3,
    DIV         = 4,
    POW         = 5,
    EXP_FUN     = 6,
    EXP         = 7,
    LOG         = 8,
    SKOBKA      = 9,
    SKOBKA_ZAKR = 10
};

enum NODE_TRIG_OPERATIONS
{
    SIN    = 1,
    COS    = 2,
    TG     = 3,
    CTG    = 4,
    ARCSIN = 5,
    ARCCOS = 6,
    ARCTG  = 7,
    ARCCTG = 8,
    SH     = 9,
    CH     = 10,
    TH     = 11,
    CTH    = 12
};

enum NODE_VARIABLES
{
    X = 0
};

enum CONSTS
{
    E = 0,
    PI = 1
};

enum NODE_STATUS
{
    NODE_NULL                     = 1,
    NODE_BAD_TYPE                 = 2,
    NODE_EMPTY_OPERATION_VALUES   = 3,
    NODE_NOT_EMPTY_VALUE_BRANCHES = 4,
    NODE_BAD_TRIG_FUNC            = 5,
    NODE_GOOD                     = 0
};

enum OPTIMIZATION
{
    YES = 0,
    NO = 1
};

struct Node 
{
    NODE_TYPE type    = EMPTY;
    int value         = 0;
    Node *left        = NULL;
    Node *right       = NULL;
};

Node* nodeCtor (NODE_TYPE nodeType, int value, Node *left, Node *right);
NODE_STATUS NodeCheckForErrors (Node *node, int line, const char* function, const char* file);
bool nodePrintError (NODE_STATUS nodeError, int line, const char* function, const char* file);
bool nodeDtor (Node *node); 

#endif