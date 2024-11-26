#include "dump.h"

bool dump (int mode, Node *node, Node *diffNode)
{
    //NodeCheckForErrors (node);

    FILE * dotFile = fopen ("dot.dot", "wb");
        if (dotFile == NULL)
        {
            return false;
        }

    fprintf (dotFile, "digraph G{\nrankdir=HR;\n");

    int rang = 0;

    treeDumpMakeNodeLabels (node, rang, dotFile, mode);
    treeDumpMakeNodeLabels (diffNode, rang, dotFile, mode);

    treeDumpMakeArrows (node, dotFile);
    treeDumpMakeArrows (diffNode, dotFile);

    fprintf (dotFile, "}");

    fclose (dotFile);

    system ("dot dot.dot -Tpng -o gr.png");

    return true;
}

bool treeDumpMakeNodeLabels (Node *node, int rang, FILE *dotFile, int mode)
{
    //checkForErrors (node);
    assert (dotFile);
    
    if (mode == NARROW)
    {
        if (node->type == NUMBER)
        {
            fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"%d\", rang = %d]\n", node, node->value, rang);
        }

        if (node->type == VARIABLE)
        {
            switch (node->value)
            {
            case X: fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"x\", rang = %d]\n", node, rang);
                    break;
            
            default: break;
            }
        }

        if (node->type == OPERATION)
        {
            switch (node->value)
            {
                case ADD:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"+\", rang = %d]\n", node, rang);
                              break;
                case SUB:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"-\", rang = %d]\n", node, rang);
                              break;
                case MUL:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"*\", rang = %d]\n", node, rang);
                              break;
                case DIV:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \":\", rang = %d]\n", node, rang);
                              break;
                case POW:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"^\", rang = %d]\n", node, rang);
                              break;
                case EXP_FUN: fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"^\", rang = %d]\n", node, rang);
                              break;
                case EXP:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"^\", rang = %d]\n", node, rang);
                              break;
                case LOG:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"log\", rang = %d]\n", node, rang);
                              break;

                default: break;
            }
        }

        if (node->type == TRIG_OPERATION)
        {
            switch (node->value)
            {
                case SIN:    fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"sin\", rang = %d]\n", node, rang);
                             break;
                case COS:    fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"cos\", rang = %d]\n", node, rang);
                             break;
                case TG:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"tg\", rang = %d]\n", node, rang);
                             break;
                case CTG:    fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"ctg\", rang = %d]\n", node, rang);
                             break;
                case ARCSIN: fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"arcsin\", rang = %d]\n", node, rang);
                             break;
                case ARCCOS: fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"arccos\", rang = %d]\n", node, rang);
                             break;
                case ARCTG:  fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"arctg\", rang = %d]\n", node, rang);
                             break;
                case ARCCTG: fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"arcctg\", rang = %d]\n", node, rang);
                             break;
                case SH:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"sh\", rang = %d]\n", node, rang);
                             break;
                case CH:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"ch\", rang = %d]\n", node, rang);
                             break;
                case TH:     fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"th\", rang = %d]\n", node, rang);
                             break;
                case CTH:    fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"cth\", rang = %d]\n", node, rang);
                             break;

                default: break;
            }
        }
    } else if (mode == WIDE)
    {
        //fprintf (dotFile, "node%p [shape=record, label = \"{%p | parent: %p | {%d} | {%p | %p}}\", rang = %d]\n", node, node, node->parent, node->value, node->left, node->right, rang);
    }

    if (node->left != NULL)
    {
        treeDumpMakeNodeLabels (node->left, rang + 1, dotFile, mode);
    }

    if (node->right != NULL)
    {
        treeDumpMakeNodeLabels (node->right, rang + 1, dotFile, mode);
    }

    return true;
}

bool treeDumpMakeArrows (Node *node, FILE *dotFile)
{
    if (node->left != NULL)
    {
        fprintf (dotFile, "node%p -> node%p\n", node, node->left);
    }
    
    if (node->right != NULL)
    {
        fprintf (dotFile, "node%p -> node%p\n", node, node->right);
    }

    if (node->left != NULL)
    {
        treeDumpMakeArrows (node->left, dotFile);
    }

    if (node->right != NULL)
    {
        treeDumpMakeArrows (node->right, dotFile);
    }

    return true;
}