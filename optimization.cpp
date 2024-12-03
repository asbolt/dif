#include "optimization.h"
#include "diff.h"

Node *nodeOptimization (Node *node)
{
    NodeCheckForErrors (node, VALUES_FOR_ERROR);

    if (node->left != NULL)
    {
        node->left = nodeOptimization (node->left);
    }

    if (node->right != NULL)
    {
        node->right = nodeOptimization (node->right);
    }

    if (node->type != OPERATION)
    {
        return copySubtree(node);
    }

    switch (node->value)
    {

    case ADD:
    case SUB:   return optimizationAddAndSub (node);
    case MUL:   return optimizationMul (node);
    case DIV: { if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(0);
                }
                return subInDiv (node);

                break; }
    case POW:   return optimizationPow (node);
    case EXP: { 
                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(0);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    FREE_OLD_BRANCHES;
                    return E_;
                }

                break;
              }

    case LOG:
    case EXP_FUN:
    default:
        return node;
    } 

    return node;
}

Node *optimizationAddAndSub (Node *node)
{
    if (node->left->type == NUMBER && node->right->type == NUMBER)
    {
        Node *sum = NUM_(node->left->value + node->right->value);
        FREE_OLD_BRANCHES;
        return sum;
    }

    if (node->right->type == NUMBER && node->right->value == 0)
    {
        Node *cl = copySubtree(node->left);
        FREE_OLD_BRANCHES;
        return cl;
    }

    if (node->value == ADD)
    {
        if (node->left->type == NUMBER && node->left->value == 0)
        {
            Node *cr = copySubtree(node->right);
            FREE_OLD_BRANCHES;
            return cr;
        }
    } else {
        if (node->left->type == NUMBER && node->left->value == 0)
        {
            Node *cr = MUL_(NUM_(-1), copySubtree(node->right));
            FREE_OLD_BRANCHES;
            return cr;
        }
    }

    return node;
}

Node *optimizationMul (Node *node)
{
    if (node->left->type == NUMBER && node->right->type == NUMBER)
    {
        Node *mul = NUM_(node->left->value * node->right->value);
        FREE_OLD_BRANCHES;
        return mul;
    }

    if (node->left->type == NUMBER && node->left->value == 0 || node->right->value == 0)
    {
        FREE_OLD_BRANCHES;
        return NUM_(0);
    }

    if (node->left->type == NUMBER && node->left->value == 1)
    {
        Node *cr = copySubtree(node->right);
        FREE_OLD_BRANCHES;
        return cr;
    }

    if (node->right->type == NUMBER && node->right->value == 1)
    {
        Node *cl = copySubtree(node->left);
        FREE_OLD_BRANCHES;
        return cl;
    }

    return node;
}

Node *optimizationPow (Node *node)
{
    if (node->left->type == NUMBER && node->right->type == NUMBER)
    {
        Node *num = NUM_(pow(node->left->value,node->right->value));
        FREE_OLD_BRANCHES;
        return num;
    }

    if (node->right->type == NUMBER && node->right->value == 0)
    {
        FREE_OLD_BRANCHES;
        return NUM_(1);
    }

    if (node->right->type == NUMBER && node->right->value == 1)
    {
        Node *cl = copySubtree(node->left);
        FREE_OLD_BRANCHES;
        return cl;
    }

    if (node->right->type == NUMBER && node->right->value < 0)
    {
        Node *div = DIV_(NUM_(1), POW_(copySubtree(node->left), NUM_(- node->right->value)));
        FREE_OLD_BRANCHES;
        return div;
    }

    return node;
}

Node *subInDiv (Node *node)
{
    if (node->value == SUB && (node->left->value == DIV || node->right->value == DIV))
    {
        if (node->left->value == DIV)
        {
            int zn = node->left->right->value;
            int ch = node->left->left->value;
            int number = node->right->value;
            int newCh = ch - zn * number;
            return DIV_(NUM_(newCh), NUM_(zn));
        } else {
            int zn = node->right->right->value;
            int ch = node->right->left->value;
            int number = node->left->value;
            int newCh = zn * number - ch;
            return DIV_(NUM_(newCh), NUM_(zn));
        }
    }

    return node;
}
