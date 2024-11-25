#include "tree.h"
#include "diff.h"
#include "dump.h"

int main ()
{
    Node *node = _EXP_FUN(_NUM(3), _X);
    Node *diffNode = diff (node);

    dump (NARROW, node, diffNode);

    nodeDtor (node);
    nodeDtor (diffNode);

    return 0;
}