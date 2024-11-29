#ifndef READ_H_
#define READ_H_

#include "diff.h"
#include "tree.h"
#include "dump.h"
#include <string.h>

Node *GetPenis (FILE *file);
Node *GetG (FILE *file);
Node *GetN (FILE *file);
Node *GetE (FILE *file);
Node *GetT (FILE *file);
Node *GetKitkat (FILE *file);
Node *GetS (FILE *file);
Node *GetL (FILE *file);
Node *SyntaxError (FILE *file);

#endif