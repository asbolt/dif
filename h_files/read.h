#ifndef READ_H_
#define READ_H_

//#include "diff.h"
#include "tree.h"
#include "dump.h"
#include <string.h>
#include <ctype.h>

struct Token
{
    NODE_TYPE type = EMPTY;
    double value   = 0;
};


Node *readFunc (Token *tokens[]);
Node *getBranches (Token *tokens[], int *p);
Node *getNumbers (Token *tokens[], int *p);
Node *getSum (Token *tokens[], int *p);
Node *getMul (Token *tokens[], int *p);
Node *getTrig (Token *tokens[], int *p);
Node *getPow (Token *tokens[], int *p);
Node *SyntaxError (Token *tokens[], int *p);
Node *getVar (Token *tokens[], int *p);
Node *makeTreeFromFile (const char* fileName);
bool makeNumberToken (char *buffer, int *i, int *n, Token *tokens[]);
bool makeOPerationToken (char *buffer, int *i, int *n, Token *tokens[]);
bool makeVarToken (char *buffer, int *i, int *n, Token *tokens[]);
bool makeFunctionToken (char *buffer, int *i, int *n, Token *tokens[]);
bool tvariUlybchatye (char *buffer, int *i, int *n, Token *tokens[]);

#endif