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

    return _NUM(val);
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
            val = _ADD(val, val2);
        else val = _SUB(val, val2);
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
            val = _MUL(val, val2);
        else val = _DIV(val, val2);
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
                return _SIN (val);
            } else if (strncmp (buffer + *p, "cos", _cos) == 0) {
                *p += _cos;
                Node *val = getPow(buffer, p);
                return _COS (val);
            } else if (strncmp (buffer + *p, "tg", _tg) == 0) {
                *p += _tg;
                Node *val = getPow(buffer, p);
                return _TG (val);
            } else if (strncmp (buffer + *p, "ctg", _ctg) == 0) {
                *p += _ctg;
                Node *val = getPow(buffer, p);
                return _CTG (val);
            } else if (strncmp (buffer + *p, "arcsin", _arcsin) == 0) {
                *p += _arcsin;
                Node *val = getPow(buffer, p);
                return _ARCSIN (val);
            } else if (strncmp (buffer + *p, "arccos", _arccos) == 0) {
                *p += _arccos;
                Node *val = getPow(buffer, p);
                return _ARCCOS (val);
            } else if (strncmp (buffer + *p, "arctg", _arctg) == 0) {
                *p += _arctg;
                Node *val = getPow(buffer, p);
                return _ARCTG (val);
            } else if (strncmp (buffer + *p, "arcctg", _arcctg) == 0) {
                *p += _arctg;
                Node *val = getPow(buffer, p);
                return _ARCCTG (val);
            } else if (strncmp (buffer + *p, "sh", _sh) == 0) {
                *p += _sh;
                Node *val = getPow(buffer, p);
                return _SH (val);
            } else if (strncmp (buffer + *p, "ch", _ch) == 0) {
                *p += _ch;
                Node *val = getPow(buffer, p);
                return _CH (val);
            } else if (strncmp (buffer + *p, "th", _th) == 0) {
                *p += _th;
                Node *val = getPow(buffer, p);
                return _TH (val);
            } else if (strncmp (buffer + *p, "cth", _cth) == 0) {
                *p += _cth;
                Node *val = getPow(buffer, p);
                return _CTH (val);
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
                        return _LOG(val,val2);
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

    if (val->type == NUMBER && val->value == E) //TODO тут явно надо исправить
    {
        return _EXP(val2);
    }

    if (val->type == NUMBER)
    {
        return _EXP_FUN(val, val2);
    }

    return _POW(val, val2);
}

Node *getVar (char *buffer, int *p)
{
    if (buffer[*p] == 'x')
    {
        (*p)++;
        return _X;
    }
    else return getNumbers (buffer, p);
}

Node *SyntaxError (char *buffer, int *p)
{
    printf ("%c\n%d\n", buffer[*p], *p);
    printf ("Syntax error\n");
    exit (1);
}