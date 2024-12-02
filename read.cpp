#include "read.h"

Node *readFunc (const char *fileName)
{
    FILE *file = fopen (fileName, "r");
    if (file == NULL)
    {
        return NULL;
    }

    fseek (file, 0, SEEK_END);
    int size = ftell (file);
    fseek (file, 0, SEEK_SET);

    char *buffer = (char *)calloc (size, sizeof(char));
    if (buffer == NULL)
    {
        return NULL;
    }
    fread (buffer, sizeof(char), size, file);

    int p = 0;

    Node* val = getSum (buffer, &p);
    if (buffer[p] != '$')
        SyntaxError (buffer, &p);

    free (buffer);
    return val;
}


Node *getBranches (char *buffer, int *p)
{
    if (buffer[*p] == '(')
    {
        (*p)++;
        Node *val = getSum (buffer, p);
        if (buffer[*p] != ')')
        {
            SyntaxError (buffer, p);
        }
        (*p)++;
        return val;
    }
    else
        return getVar (buffer, p);
}

Node *getNumbers (char *buffer, int *p)
{
    int val = 0;
    int pOld = *p;
    while ('0' <= buffer[*p] && buffer[*p] <= '9')
    {
        val = val*10 + buffer[*p] - '0';
        (*p)++;
    }

    if (pOld == *p)
        SyntaxError (buffer, p);

    return NUM_(val);
}

Node *getSum (char *buffer, int *p)
{
    Node *val = getMul (buffer, p);
    while (buffer[*p] == '+' || buffer[*p] == '-')
    {
        int op = buffer[*p];
        (*p)++;
        Node *val2 = getMul (buffer, p);
        if (op == '+')
            val = ADD_(val, val2);
        else val = SUB_(val, val2);
    }
    return val;
}

Node *getMul (char *buffer, int *p)
{
    Node *val = getTrig (buffer, p);

    while (buffer[*p] == '*' || buffer[*p] == '/')
    {
        int op = buffer[*p];
        (*p)++;
        Node *val2 = getTrig (buffer, p);
        if (op == '*')
            val = MUL_(val, val2);
        else val = DIV_(val, val2);
    }
    return val;
}

Node *getTrig (char *buffer, int *p)
{
    if (strncmp (buffer + *p, "sin", _sin) == 0 ||
        strncmp (buffer + *p, "cos", _cos) == 0 ||
        strncmp (buffer + *p, "tg", _tg) == 0 ||
        strncmp (buffer + *p, "ctg", _ctg) == 0 ||
        strncmp (buffer + *p, "arcsin", _arcsin) == 0 ||
        strncmp (buffer + *p, "arccos", _arccos) == 0 ||
        strncmp (buffer + *p, "arctg", _arctg) == 0 ||
        strncmp (buffer + *p, "arcctg", _arcctg) == 0 ||
        strncmp (buffer + *p, "sh", _sh) == 0 ||
        strncmp (buffer + *p, "ch", _ch) == 0 ||
        strncmp (buffer + *p, "th", _th) == 0 ||
        strncmp (buffer + *p, "cth", _cth) == 0 ||
        strncmp (buffer + *p, "log", _log) == 0)
        {

            if (strncmp (buffer + *p, "sin", _sin) == 0)
            {
                *p += _sin;
                Node *val = getPow(buffer, p);
                return SIN_ (val);
            } else if (strncmp (buffer + *p, "cos", _cos) == 0) {
                *p += _cos;
                Node *val = getPow(buffer, p);
                return COS_ (val);
            } else if (strncmp (buffer + *p, "tg", _tg) == 0) {
                *p += _tg;
                Node *val = getPow(buffer, p);
                return TG_ (val);
            } else if (strncmp (buffer + *p, "ctg", _ctg) == 0) {
                *p += _ctg;
                Node *val = getPow(buffer, p);
                return CTG_ (val);
            } else if (strncmp (buffer + *p, "arcsin", _arcsin) == 0) {
                *p += _arcsin;
                Node *val = getPow(buffer, p);
                return ARCSIN_ (val);
            } else if (strncmp (buffer + *p, "arccos", _arccos) == 0) {
                *p += _arccos;
                Node *val = getPow(buffer, p);
                return ARCCOS_ (val);
            } else if (strncmp (buffer + *p, "arctg", _arctg) == 0) {
                *p += _arctg;
                Node *val = getPow(buffer, p);
                return ARCTG_ (val);
            } else if (strncmp (buffer + *p, "arcctg", _arcctg) == 0) {
                *p += _arctg;
                Node *val = getPow(buffer, p);
                return ARCCTG_ (val);
            } else if (strncmp (buffer + *p, "sh", _sh) == 0) {
                *p += _sh;
                Node *val = getPow(buffer, p);
                return SH_ (val);
            } else if (strncmp (buffer + *p, "ch", _ch) == 0) {
                *p += _ch;
                Node *val = getPow(buffer, p);
                return CH_ (val);
            } else if (strncmp (buffer + *p, "th", _th) == 0) {
                *p += _th;
                Node *val = getPow(buffer, p);
                return TH_ (val);
            } else if (strncmp (buffer + *p, "cth", _cth) == 0) {
                *p += _cth;
                Node *val = getPow(buffer, p);
                return CTH_ (val);
            } else if (strncmp (buffer + *p, "log", _log) == 0)
            {
                *p += _log;
                Node *val = getNumbers (buffer, p);
                if (buffer[*p] == '(')
                {
                    (*p)++;
                    Node *val2 = getSum (buffer, p);
                    if (buffer[*p] == ')')
                    {
                        (*p)++;
                        return LOG_(val,val2);
                    }
                    SyntaxError (buffer, p);
                } else
                {
                    SyntaxError (buffer, p);
                }
            }
            
            return NULL;
        }
    else return getPow (buffer, p);
}

Node *getPow (char *buffer, int *p)
{
    Node *val = getBranches (buffer, p);
    Node *val2 = NULL;

    if (buffer[*p] == '^' )
    {
        (*p)++;
        val2 = getBranches (buffer, p);
    } else {
        return val;
    }

    if (val->type == CONST && val->value == E)
    {
        return EXP_(val2);
    }

    if (val->type == NUMBER)
    {
        return EXP_FUN_(val, val2);
    }

    return POW_(val, val2);
}

Node *getVar (char *buffer, int *p)
{
    if (buffer[*p] == 'x')
    {
        (*p)++;
        return X_;
    } else if (buffer[*p] == 'e')
    {
        (*p)++;
        return E_;
    } else if (strncmp (buffer + *p, "pi", _pi) == 0)
    {
        (*p) += _pi;
        return PI_;
    }
    else return getNumbers (buffer, p);
}

Node *SyntaxError (char *buffer, int *p)
{
    printf ("%c\n%d\n", buffer[*p], *p);
    printf ("Syntax error\n");
    exit (1);
}