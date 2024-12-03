#ifndef DIFF_H_
#define DIFF_H_

#include "optimization.h"

Node *differentiateAndOptimize(Node *root, FILE *file);
Node *copySubtree (Node *node);
Node *differentiate (Node *node, Node *root, FILE *file);
bool makeTexFile (const char *fileName, Node *node);
bool printFunc (FILE *file, Node *node, Node *diffNode);

#endif