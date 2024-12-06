#include "read.h"

Node *readFunc (Token *tokens[])
{
    int p = 0;

    Node* val = getSum (tokens, &p);
    if (tokens[p]->type != EMPTY)
    {
        SyntaxError (tokens, &p);
    }

    return val;
}


Node *getBranches (Token *tokens[], int *p)
{
    if (tokens[*p]->type == OPERATION && tokens[*p]->value == SKOBKA)
    {
        (*p)++;
        Node *val = getSum (tokens, p);
        if (tokens[*p]->type != OPERATION && tokens[*p]->value != SKOBKA_ZAKR)
        {
            SyntaxError (tokens, p);
        }
        (*p)++;
        return val;
    }
    else
        return getVar (tokens, p);
}

Node *getNumbers (Token *tokens[], int *p)
{
    (*p)++;
    return NUM_(tokens[*p - 1]->value);
}

Node *getSum (Token *tokens[], int *p)
{
    Node *val = getMul (tokens, p);
    while (tokens[*p]->type == OPERATION && (tokens[*p]->value == ADD || tokens[*p]->value == SUB))
    {
        int op = tokens[*p]->value;
        (*p)++;
        Node *val2 = getMul (tokens, p);
        if (op == ADD)
            val = ADD_(val, val2);
        else val = SUB_(val, val2);
    }
    return val;
}

Node *getMul (Token *tokens[], int *p)
{
    Node *val = getTrig (tokens, p);

    while (tokens[*p]->type == OPERATION && (tokens[*p]->value == MUL || tokens[*p]->value == DIV))
    {
        int op = tokens[*p]->value;
        (*p)++;
        Node *val2 = getTrig (tokens, p);
        if (op == MUL)
            val = MUL_(val, val2);
        else val = DIV_(val, val2);
    }
    return val;
}

Node *getTrig (Token *tokens[], int *p)
{
    if (tokens[*p]->type == TRIG_OPERATION)
        {
            int operation = tokens[*p]->value;
            (*p)++;
            Node *val = getPow(tokens, p);
            return nodeCtor(TRIG_OPERATION, operation, val, NULL);
        } else if (tokens[*p]->type == OPERATION && tokens[*p]->value == LOG) {
            (*p)++;
            Node *val = getNumbers (tokens, p);
            if (tokens[*p]->type == OPERATION && tokens[*p]->value == SKOBKA)
            {
                (*p)++;
                Node *val2 = getSum (tokens, p);
                if (tokens[*p]->type == OPERATION && tokens[*p]->value == SKOBKA_ZAKR)
                {
                    (*p)++;
                    return LOG_(val,val2);
                }
                SyntaxError (tokens, p);
                return NULL;
            } else {
                SyntaxError (tokens, p);
                return NULL;
            }
        }
    else return getPow (tokens, p);
}

Node *getPow (Token *tokens[], int *p)
{
    Node *val = getBranches (tokens, p);
    Node *val2 = NULL;

    if (tokens[*p]->type == OPERATION && tokens[*p]->value == POW)
    {
        (*p)++;
        val2 = getBranches (tokens, p);
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

Node *getVar (Token *tokens[], int *p)
{
    if (tokens[*p]->type == VARIABLE && tokens[*p]->value == X)
    {
        (*p)++;
        return X_;
    } else if (tokens[*p]->type == CONST && tokens[*p]->value == E)
    {
        (*p)++;
        return E_;
    } else if (tokens[*p]->type == CONST && tokens[*p]->value == PI)
    {
        (*p)++;
        return PI_;
    }
    else return getNumbers (tokens, p);
}

Node *SyntaxError (Token *tokens[], int *p)
{
    printf ("Syntax error ");
    printf ("%d %f\n", tokens[*p]->type, tokens[*p]->value);
    exit (1);
}

Node * makeTreeFromFile (const char* fileName)
{
    FILE *file = fopen (fileName, "r");
    if (file == NULL)
    {
        return 0;
    }

    fseek (file, 0, SEEK_END);
    int size = ftell (file);
    fseek (file, 0, SEEK_SET);

    char *buffer = (char *)calloc (size, sizeof(char));
    if (buffer == NULL)
    {
        return 0;
    }

    fread (buffer, sizeof(char), size, file);
    fclose (file);

    Token **tokens = (Token **)calloc (size, sizeof(Token*));
    if (tokens == NULL)
    {
        return 0;
    }
    for (int y = 0; y < size; y++)
    {
        tokens[y] = (Token*) calloc(1, sizeof(Token));
    }

    int i = 0;
    int n = 0;
    while (*(buffer + i) != '$')
    {
        if (*(buffer + i) == ' ')
        {
            i++;
        } else if (isdigit (*(buffer + i)) != 0)
        {
            makeNumberToken (buffer, &i, &n, tokens);
        } else if (isalpha (*(buffer + i)) != 0)
        {
            makeFunctionToken (buffer, &i, &n, tokens);
        } else {
            makeOPerationToken (buffer, &i, &n, tokens);
        }
    }
    tokens[n]->type = EMPTY;
    n++;

    /*for (int u = 0; u < n; u++)
    {
        printf ("%d %f\n", tokens[u]->type, tokens[u]->value);
    }*/

    Node* node = readFunc(tokens);

    dump (WIDE, node, node);

    return node;
}

bool makeNumberToken (char *buffer, int *i, int *n, Token *tokens[])
{
    double value = 0;
    while (isdigit (*(buffer + *i)) != 0)
    {
        value = value*10 + *(buffer + *i) - '0';
        (*i)++;
    }

    if (*(buffer + *i) == ',')
    {
        (*i)++;
        if (isdigit (*(buffer + *i)) == 0)
        {
            SyntaxError (tokens, n);
            return false;
        }

        int oldI = *i;
        while (isdigit (*(buffer + *i)) != 0)
        {
            value = value*10 + *(buffer + *i) - '0';
            (*i)++;
        }
        value = value / (pow(10, *i - oldI));
    }

    tokens[*n]->type = NUMBER;
    tokens[*n]->value = value;
    (*n)++;

    return true;
}

bool makeFunctionToken (char *buffer, int *i, int *n, Token *tokens[])
{
   const char* func[][3] = {{"sin", "1", "3"},
                            {"cos", "2", "3"},
                            {"tg", "3", "2"},
                            {"ctg", "4", "3"},
                            {"arcsin", "5", "6"},
                            {"arccos", "6", "6"},
                            {"arctg", "7", "5"},
                            {"arcctg", "8", "6"},
                            {"sh", "9", "2"},
                            {"ch", "10", "2"},
                            {"th", "11", "2"},
                            {"cth", "12", "3"}};

    
    for (int o = 1; o <= sizeof(func)/sizeof(const char *[3]); o++)
    {
        if (strncmp(buffer + *i, func[o-1][0], *(func[o-1][2]) - '0') == 0)
        {
            tokens[*n]->type = TRIG_OPERATION;
            tokens[*n]->value = o;
            (*n)++;
            (*i) += *(func[o-1][2]) - '0';
            return true;
        }
    }

    if (strncmp(buffer + *i, "log", 3) == 0)
        {
            tokens[*n]->type = OPERATION;
            tokens[*n]->value = 8;
            (*n)++;
            (*i) += 3;
            return true;
        }

    makeVarToken (buffer, i, n, tokens);
    
    return false;
}

bool makeVarToken (char *buffer, int *i, int *n, Token *tokens[])
{
    if (strchr(buffer + *i, 'x') != 0)
    {
        tokens[*n]->type = VARIABLE;
        tokens[*n]->value = 0;
        (*n)++;
        (*i)++;
        return true;
    } else if (strchr(buffer + *i, 'e') != 0)
    {
        tokens[*n]->type = CONST;
        tokens[*n]->value = E;
        (*n)++;
        (*i)++;
        return true;
    } else if (strncmp(buffer + *i, "pi", 2) == 0)
    {
        tokens[*n]->type = CONST;
        tokens[*n]->value = PI;
        (*n)++;
        (*i) += 2;
        return true;
    }

    return false;
}

bool makeOPerationToken (char *buffer, int *i, int *n, Token *tokens[])
{
    const char operation [] =  {'+',
                                '-',
                                '*',
                                '/',
                                '^'};

    
    for (int l = 1; l <= sizeof(operation)/sizeof(const char); l++)
    {
        if (*(buffer + *i) == operation[l - 1])
        {
            tokens[*n]->type = OPERATION;
            tokens[*n]->value = l;
            (*n)++;
            (*i)++;
            return true;
        }
    }

    return tvariUlybchatye (buffer, i, n, tokens);
}

bool tvariUlybchatye (char *buffer, int *i, int *n, Token *tokens[])
{
    if (strchr(buffer + *i, '(') != 0)
        {
            tokens[*n]->type = OPERATION;
            tokens[*n]->value = SKOBKA;
            (*n)++;
            (*i)++;
            return true;
        }

    if (strchr(buffer + *i, ')') != 0)
        {
            tokens[*n]->type = OPERATION;
            tokens[*n]->value = SKOBKA_ZAKR;
            (*n)++;
            (*i)++;
            return true;
        }

    return false;
}