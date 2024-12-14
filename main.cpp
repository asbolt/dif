#include "h_files/tree.h"
#include "h_files/diff.h"
#include "h_files/dump.h"
#include "h_files/read.h"

int main ()
{
    Node *node = makeTreeFromFile ("func.txt");
    makeTexFile ("tex.tex", node);
    nodeDtor (node);

    return 0;
}