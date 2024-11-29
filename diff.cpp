#include "diff.h"
#include "dump.h"

Node *diff (Node *node)
{
    NodeCheckForErrors (node, __LINE__, __FUNCTION__, __FILE__);

    Node *diffNode = makeDiff (node);
    diffNode = nodeOptimization (diffNode);

    return diffNode;
}

Node *makeDiff (Node *node)
{
    NodeCheckForErrors(node, __LINE__, __FUNCTION__, __FILE__);

    if (node->type == NUMBER)
    {
        return _NUM(0); 
    }

    if (node->type == VARIABLE)
    {
        return _NUM(1);
    }

    if (node->type == OPERATION)
    {
        switch (node->value)
        {
            case ADD:     return _ADD(diff (node->left), diff (node->right));
            case SUB:     return _SUB(diff (node->left), diff (node->right));
            case MUL:     {
                            Node *dl = diff (node->left);
                            Node *dr = diff (node->right);
                            Node *l = copy (node->left);
                            Node *r = copy (node->right);
                            return _ADD(_MUL(dl, r), _MUL(l, dr)); 
                          }
            case DIV:     {
                            Node *dl = diff (node->left);
                            Node *dr = diff (node->right);
                            Node *l = copy (node->left);
                            Node *r = copy (node->right);
                            Node *numerator = _SUB(_MUL(dl, r), _MUL(l, dr));
                            Node *rDet = copy (node->right);
                            Node *denominator = _POW(rDet, _NUM(2));
                            return _DIV(numerator, denominator);
                          }
            case POW:     {
                            Node *ind = copy (node->right);
                            Node *base = _POW(_X, _NUM(node->right->value - 1));
                            return _MUL(base, ind);
                          }
            case EXP_FUN: {
                            Node *base = copy (node);
                            Node *ln = _LOG(_NUM(E), _NUM(node->left->value));
                            return _MUL(base, ln);
                          }
            case EXP:     {
                            Node *exp = copy (node);
                            Node *ind = diff (node->right);
                            return _MUL(exp, ind);
                          }
            case LOG:     {
                            Node *num = _DIV(_NUM(1), _MUL(copy (node->right), _LOG(_NUM(E), copy (node->left))));
                            return _MUL(num, diff(node->right));
                          }

            default: break;
        }
    }

    if (node->type == TRIG_OPERATION)
    {
        switch (node->value)
        {
            case SIN:    return _MUL(_COS(copy (node->left)), diff (node->left));
            case COS:    return _MUL(_SUB(_NUM(0), _SIN(copy (node->left))), diff (node->left));
            case TG:     {
                           Node *denominator = _POW(_COS(copy (node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           return _MUL(div, diff (node->left)); 
                         }
            case CTG:    {
                           Node *denominator = _POW(_SIN(copy (node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           return _MUL(div, diff (node->left));
                         }
            case ARCSIN: {
                           Node *base = _SUB(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           return _DIV(_NUM(1), denominator);
                         }
            case ARCCOS: {
                           Node *base = _SUB(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           return _SUB(_NUM(0), _DIV(_NUM(1), denominator));
                         }
            case ARCTG:  {
                           Node *base = _ADD(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           return _DIV(_NUM(1), base);
                         }
            case ARCCTG: {
                           Node *base = _ADD(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           return _SUB(_NUM(0), _DIV(_NUM(1), base));
                         }
            case SH:     return _MUL(_CH(copy (node->left)), diff (node->left));
            case CH:     return _MUL(_SH(copy (node->left)), diff (node->left));
            case TH:     {
                           Node *denominator = _POW(_CH(copy (node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           return _MUL(div, diff(node->left)); 
                         }
            case CTH:    {
                           Node *denominator = _POW(_SH(copy (node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           return _MUL(div, diff (node->left));
                         }

            default: break;
        }
    }

    return NULL;
}

Node *copy (Node *node)
{
    NodeCheckForErrors(node, __LINE__, __FUNCTION__, __FILE__);

    Node *cl = NULL;
    Node *cr = NULL;

    if (node->left != NULL)
    {
        cl = copy (node->left);
    }

    if (node->right != NULL)
    {
        cr = copy (node->right);
    }

    Node *nodeCopy = nodeCtor (node->type, node->value, cl, cr);

    return nodeCopy; 
}

Node *nodeOptimization (Node *node)
{
    NodeCheckForErrors (node, __LINE__, __FUNCTION__, __FILE__);

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
        return copy(node);
    }

    switch (node->value)
    {
    case ADD: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *sum = _NUM(node->left->value + node->right->value);
                    FREE_OLD_BRANCHES;
                    return sum;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    Node *cr = copy(node->right);
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                break;
              }
    case SUB: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *sum = _NUM(node->left->value + node->right->value);
                    FREE_OLD_BRANCHES;
                    return sum;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    Node *cr = _MUL(_NUM(-1), copy(node->right));
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                break;
              }
    case MUL: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *mul = _NUM(node->left->value * node->right->value);
                    FREE_OLD_BRANCHES;
                    return mul;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->left->type == NUMBER && node->left->value == 1)
                {
                    Node *cr = copy(node->right);
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }
              }
    case DIV: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *div = _NUM(node->left->value / node->right->value);
                    FREE_OLD_BRANCHES;
                    return div;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                break;
              }
    case POW: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *num = _NUM(pow(node->left->value,node->right->value));
                    FREE_OLD_BRANCHES;
                    return num;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(1);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                if (node->right->type == NUMBER && node->right->value < 0)
                {
                    Node *div = _DIV(_NUM(1), _POW(copy(node->left), _NUM(- node->right->value)));
                    FREE_OLD_BRANCHES;
                    return div;
                }

                break;
              }
    case EXP: { 
                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(E);
                }

                break;
              }

    case LOG:
    case EXP_FUN:
    default:
        return copy(node);
    } 

    return copy(node);
}