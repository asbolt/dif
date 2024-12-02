#include "tree.h"
#include "diff.h"
#include "dump.h"
#include "read.h"

int main ()
{
    Node *node = readFunc ("func.txt");
    makeTexFile ("tex.tex", node);
    nodeDtor (node);
    //nodeDtor (diffNode); // TODO

    return 0;
}