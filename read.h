#ifndef READ_H_
#define READ_H_

#include "diff.h"
#include "tree.h"
#include "dump.h"
#include <string.h>

Node *readFunc (const char *fileName);
Node *getBranches (char *buffer, int *p);
Node *getNumbers (char *buffer, int *p);
Node *getSum (char *buffer, int *p);
Node *getMul (char *buffer, int *p);
Node *getTrig (char *buffer, int *p);
Node *getPow (char *buffer, int *p);
Node *SyntaxError (char *buffer, int *p);
Node *getVar (char *buffer, int *p);

#endif