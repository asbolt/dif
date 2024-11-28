#include "tree.h"
#include "diff.h"
#include "dump.h"

int main ()
{
    Node *node = _EXP(_ADD(_X, _SIN(_X)));
    Node *diffNode = diff (node);
    dump (NARROW, node, diffNode);

    Node *optNode = nodeOptimization (diffNode);

    dump (NARROW, node, optNode);

    texPrint (diffNode, "tex.tex");

    nodeDtor (node);
    nodeDtor (diffNode);

    return 0;
}