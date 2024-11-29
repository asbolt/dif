#include "tree.h"

Node* nodeCtor (NODE_TYPE nodeType, int value, Node *left, Node *right)
{
    Node *node = (Node *)calloc (1, sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }

    node->type = nodeType;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

NODE_STATUS NodeCheckForErrors (Node *node, int line, const char* function, const char* file)
{
    if (node == NULL)
    {
        nodePrintError (NODE_NULL, line, function, file);
        return NODE_NULL;
    }

    switch (node->type)
    {
    case NUMBER:
    case VARIABLE:
    case OPERATION:
    case CONST:
    case TRIG_OPERATION: break;
    
    default:        nodePrintError (NODE_BAD_TYPE, line, function, file);
                    return NODE_BAD_TYPE;
                    break;
    }

    if (node->type == OPERATION && (node->left == NULL || node->right == NULL))
    {
        nodePrintError (NODE_EMPTY_OPERATION_VALUES, line, function, file);
        return NODE_EMPTY_OPERATION_VALUES;
    }

    if (node->type == TRIG_OPERATION && (node->left == NULL || node->right != NULL))
    {
        nodePrintError (NODE_BAD_TRIG_FUNC, line, function, file);
        return NODE_BAD_TRIG_FUNC;
    }

    if ((node->type == NUMBER || node->type == VARIABLE || node->type == CONST) && (node->left != NULL || node->right != NULL))
    {
        nodePrintError (NODE_NOT_EMPTY_VALUE_BRANCHES, line, function, file);
        return NODE_NOT_EMPTY_VALUE_BRANCHES;
    }

    return NODE_GOOD;
}

bool nodePrintError (NODE_STATUS nodeError, int line, const char* function, const char* file)
{
    if (nodeError == NODE_GOOD)
    {
        return true;
    }

    printf ("Error in file %s:%d (function %s): ", file, line, function);

    switch (nodeError)
    {
        case NODE_NULL:                     printf ("null pointer of node\n");
                                            return false;

        case NODE_BAD_TYPE:                 printf ("unknown node type\n");
                                            return false;

        case NODE_EMPTY_OPERATION_VALUES:   printf ("node operation has no operands\n");
                                            return false;

        case NODE_NOT_EMPTY_VALUE_BRANCHES: printf ("number has operands\n");
                                            return false;
        case NODE_BAD_TRIG_FUNC:            printf ("trigonometry function has null or two arguments\n");
                                            return false;

        case NODE_GOOD:
        default:                            return true;
    }
}

bool nodeDtor (Node *node)
{
    assert (node);

    if (node->right != NULL)
    {
        nodeDtor (node->left);
    }

    if (node->right != NULL)
    {
        nodeDtor (node->right);
    }

    free (node);
    node = NULL;

    return true;
}
