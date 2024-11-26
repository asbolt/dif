#include "tree.h"
#include "diff.h"
#include "dump.h"

int main ()
{
    Node *node = _ADD(_X, _NUM(0));
    Node *diffNode = diff (node);
    diffNode = nodeOptimization (diffNode);

    dump (NARROW, node, diffNode);

    nodeDtor (node);
    nodeDtor (diffNode);

    return 0;
}