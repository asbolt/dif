#include "tree.h"
#include "diff.h"
#include "dump.h"
#include "read.h"

int main ()
{
    Node *node = readFunc ("func.txt");
    //Node *node = _POW(_X, _NUM(1));
    Node *diffNode = diff (node);
    dump (NARROW, node, diffNode);

    //Node *optNode = nodeOptimization (diffNode);
    node = nodeOptimization (node);
    //diffNode = nodeOptimization (diffNode);

    dump (NARROW, node, diffNode);

    texPrint (diffNode, "tex.tex");

    nodeDtor (node);
    nodeDtor (diffNode);

    return 0;
}