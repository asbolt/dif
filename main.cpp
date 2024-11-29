#include "tree.h"
#include "diff.h"
#include "dump.h"
#include "read.h"

int main ()
{
    Node *node = readFunc ("func.txt");
    //Node *node = _EXP(_ADD(_X, _SIN(_X)));
    Node *diffNode = diff (node);
    dump (NARROW, node, diffNode);

    //Node *optNode = nodeOptimization (diffNode);
    node = nodeOptimization (node);

    dump (NARROW, node, diffNode);

    texPrint (diffNode, "tex.tex");

    nodeDtor (node);
    nodeDtor (diffNode);

    return 0;
}