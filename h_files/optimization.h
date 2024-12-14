#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "tree.h"
#include "dsl.h"
#include <math.h>

#define FREE_OLD_BRANCHES free (node->left);\
                          node->left = NULL;\
                          free (node->right);\
                          node->right = NULL\

Node *nodeOptimization (Node *node);
Node *optimizationAddAndSub (Node *node);
Node *optimizationMul (Node *node);
Node *optimizationPow (Node *node);
Node *subInDiv (Node *node);

#endif