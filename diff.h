#ifndef DIFF_H_
#define DIFF_H_

#include "tree.h"
#include "dsl.h"
#include <math.h>
#include <time.h>

#define FREE_OLD_BRANCHES free (node->left);\
                          node->left = NULL;\
                          free (node->right);\
                          node->right = NULL\

Node *diff (Node *root, FILE *file);
Node *copy (Node *node);
Node *nodeOptimization (Node *node);
Node *makeDiff (Node *node, Node *root, FILE *file);
bool makeTexFile (const char *fileName, Node *node);
bool printFunc (FILE *file, Node *node, Node *diffNode);

#endif