#include "tree.h"
#include "diff.h"
#include "dump.h"
#include "read.h"

int main ()
{
    Node *node = makeTreeFromFile ("func.txt");
    makeTexFile ("tex.tex", node);
    nodeDtor (node);

    return 0;
}