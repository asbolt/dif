#include "diff.h"

Node *diff (Node *node)
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
                            Node *ind = copy (node->right);
                            Node *base = _POW(_X, _NUM(node->right->value - 1));
                            return _POW(base, ind);
                          }
            case EXP:     {
                            Node *exp = copy (node);
                            Node *ind = diff (node->right);
                            return _MUL(exp, ind);
                          }
            case LOG:     {
                            Node *num = _DIV(_NUM(1), _MUL(copy(node->right), _LOG(_NUM(E), node->right)));
                            return _MUL(num, diff(node->right));
                          }

            default: break;
        }
    }

    if (node->type == TRIG_OPERATION)
    {
        switch (node->value)
        {
            case SIN:    return _COS(diff (node->left));
            case COS:    return _SUB(_NUM(0), _SIN(diff (node->left)));
            case TG:     {
                           Node *denominator = _POW(_COS(copy(node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           return _MUL(div, diff(node->left)); 
                         }
            case CTG:    {
                           Node *denominator = _POW(_SIN(copy(node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           return _MUL(div, diff(node->left));
                         }
            case ARCSIN: {
                           Node *base = _SUB(_NUM(1), (_POW(copy(node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           return _DIV(_NUM(1), denominator);
                         }
            case ARCCOS: {
                           Node *base = _SUB(_NUM(1), (_POW(copy(node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           return _SUB(_NUM(0), _DIV(_NUM(1), denominator));
                         }
            case ARCTG:  {
                           Node *base = _ADD(_NUM(1), (_POW(copy(node->left), _NUM(2))));
                           return _DIV(_NUM(1), base);
                         }
            case ARCCTG: {
                           Node *base = _ADD(_NUM(1), (_POW(copy(node->left), _NUM(2))));
                           return _SUB(_NUM(0), _DIV(_NUM(1), base));
                         }
            case SH:     return _CH(diff (node->left));
            case CH:     return _SH(diff (node->left));
            case TH:     {
                           Node *denominator = _POW(_CH(copy(node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           return _MUL(div, diff(node->left)); 
                         }
            case CTH:    {
                           Node *denominator = _POW(_SH(copy(node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           return _MUL(div, diff(node->left));
                         }

            default: break;
        }
    }

    return NULL;
}

Node *copy (Node *node)
{
    //NodeCheckForErrors(node, __LINE__, __FUNCTION__, __FILE__);
    if (node == NULL)
    {
        return NULL;
    }

    Node *nodeCopy = nodeCtor (node->type, node->value, copy(node->left), copy(node->right));

    return nodeCopy; 
}