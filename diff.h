#ifndef DIFF_H_
#define DIFF_H_

#include "tree.h"
#include "dsl.h"
#include <math.h>

#define FREE_OLD_BRANCHES free (node->left);\
                          node->left = NULL;\
                          free (node->right);\
                          node->right = NULL\

Node *diff (Node *node);
Node *copy (Node *node);
Node *nodeOptimization (Node *node);
Node *makeDiff (Node *node);

#endif