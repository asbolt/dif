#include "read.h"

const char *s = "1+(1+2*2)*2+sin(8+1)$";
int p = 0;

int main ()
{
    Node *node = GetG();

    dump (NARROW, node, node);

    return 0;
}

Node *GetG ()
{
    Node* val = GetE ();
    if (s[p] != '$')
        SyntaxError ();
    return val;
}


Node *GetPenis ()
{
    if (s[p] == '(')
    {
        p++;
        Node *val = GetE ();
        if (s[p] != ')')
        {
            SyntaxError ();
        }
        p++;
        return val;
    }
    else
        return GetN ();
}

Node *GetN ()
{
    int val = 0;
    int pOld = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val*10 + s[p] - '0';
        p++;
    }

    if (pOld == p)
        SyntaxError ();

    return _NUM(val);
}

Node *GetE ()
{
    Node *val = GetT ();
    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        Node *val2 = GetT ();
        if (op == '+')
            val = _ADD(val, val2);
        else val = _SUB(val, val2);
    }
    return val;
}

Node *GetT ()
{
    Node *val = GetKitkat ();

    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        Node *val2 = GetKitkat ();
        if (op == '*')
            val = _MUL(val, val2);
        else val = _DIV(val, val2);
    }
    return val;
}

Node *GetKitkat ()
{
    if (strncmp (s + p, "sin", _sin) == 0 ||
        strncmp (s + p, "cos", _cos) == 0 ||
        strncmp (s + p, "tg", _tg) == 0 ||
        strncmp (s + p, "ctg", _ctg) == 0 ||
        strncmp (s + p, "arcsin", _arcsin) == 0 ||
        strncmp (s + p, "arccos", _arccos) == 0 ||
        strncmp (s + p, "arctg", _arctg) == 0 ||
        strncmp (s + p, "arcctg", _arcctg) == 0 ||
        strncmp (s + p, "sh", _sh) == 0 ||
        strncmp (s + p, "ch", _ch) == 0 ||
        strncmp (s + p, "th", _th) == 0 ||
        strncmp (s + p, "cth", _cth) == 0 )
        {

            if (strncmp (s + p, "sin", _sin) == 0)
            {
                p += _sin;
                Node *val = GetPenis();
                return _SIN (val);
            } else if (strncmp (s + p, "cos", _cos) == 0) {
                p += _cos;
                Node *val = GetPenis();
                return _COS (val);
            } else if (strncmp (s + p, "tg", _tg) == 0) {
                p += _tg;
                Node *val = GetPenis();
                return _TG (val);
            } else if (strncmp (s + p, "ctg", _ctg) == 0) {
                p += _ctg;
                Node *val = GetPenis();
                return _CTG (val);
            } else if (strncmp (s + p, "arcsin", _arcsin) == 0) {
                p += _arcsin;
                Node *val = GetPenis();
                return _ARCSIN (val);
            } else if (strncmp (s + p, "arccos", _arccos) == 0) {
                p += _arccos;
                Node *val = GetPenis();
                return _ARCCOS (val);
            } else if (strncmp (s + p, "arctg", _arctg) == 0) {
                p += _arctg;
                Node *val = GetPenis();
                return _ARCTG (val);
            } else if (strncmp (s + p, "arcctg", _arcctg) == 0) {
                p += _arctg;
                Node *val = GetPenis();
                return _ARCCTG (val);
            } else if (strncmp (s + p, "sh", _sh) == 0) {
                p += _sh;
                Node *val = GetPenis();
                return _SH (val);
            } else if (strncmp (s + p, "ch", _ch) == 0) {
                p += _ch;
                Node *val = GetPenis();
                return _CH (val);
            } else if (strncmp (s + p, "th", _th) == 0) {
                p += _th;
                Node *val = GetPenis();
                return _TH (val);
            } else if (strncmp (s + p, "cth", _cth) == 0) {
                p += _cth;
                Node *val = GetPenis();
                return _CTH (val);
            }
            
            return NULL;
        }
    else return GetPenis ();
}

Node *SyntaxError ()
{
    printf ("%c", s[p]);
    printf ("Syntax error\n");

    return 0;
} 