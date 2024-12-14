#ifndef DUMP_H_
#define DUMP_H_

#include "diff.h"
#include <stdlib.h>

enum MODE
{
    WIDE,
    NARROW
};

bool dump (int mode, Node *node, Node *diffNode);
bool treeDumpMakeNodeLabels (Node *node, int rang, FILE *dotFile, int mode);
bool treeDumpMakeArrows (Node *node, FILE *dotFile);
bool textPrintNodes (Node *node, FILE *file);
bool texPrint (Node* node, const char *fileName);

#endif
