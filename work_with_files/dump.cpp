#include "../h_files/dump.h"

bool dump (int mode, Node *node, Node *diffNode)
{
    NodeCheckForErrors (node, VALUES_FOR_ERROR);

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
    assert (dotFile);
    NodeCheckForErrors (node, VALUES_FOR_ERROR);
    
    if (mode == NARROW)
    {
        if (node->type == NUMBER)
        {
            fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"%d\", rang = %d]\n", node, node->value, rang);
        }

        if (node->type == CONST)
        {
            if (node->value == E)
                fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"e\", rang = %d]\n", node, rang);
            else
                fprintf (dotFile, "node%p [shape=\"rectangle\", label = \"pi\", rang = %d]\n", node, rang);
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
        if (node->type == NUMBER)
        {
            fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  %d|{NULL | NULL}}\", rang = %d]\n", node, 
                     node, node->type, node->value, node->value, rang);
        }

        if (node->type == CONST)
        {
            if (node->value == E)
                fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  e|{%p | %p}}\", rang = %d]\n", node, 
                         node, node->type, node->value,node->left, node->right, rang);
            else
                fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  pi|{%p | %p}}\", rang = %d]\n", node, 
                         node, node->type, node->value,node->left, node->right, rang);
        }

        if (node->type == VARIABLE)
        {
            switch (node->value)
            {
            case X: fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  x|{%p | %p}}\", rang = %d]\n", node, 
                             node, node->type, node->value,node->left, node->right, rang);
                    break;
            
            default: break;
            }
        }

        if (node->type == OPERATION)
        {
            switch (node->value)
            {
                case ADD:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  +|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case SUB:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  -|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case MUL:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  *|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case DIV:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  :|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case POW:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  ^|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case EXP_FUN: fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  ^|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case EXP:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  ^|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;
                case LOG:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d |  log|{%p | %p}}\", rang = %d]\n", node, 
                                       node, node->type, node->value,node->left, node->right, rang);
                              break;

                default: break;
            }
        }

        if (node->type == TRIG_OPERATION)
        {
            switch (node->value)
            {
                case SIN:    fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | sin|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case COS:    fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | cos|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case TG:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | tg|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case CTG:    fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | ctg|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case ARCSIN: fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | arcsin|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case ARCCOS: fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | arccos|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case ARCTG:  fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | arctg|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case ARCCTG: fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | arcctg|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case SH:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | sh|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case CH:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | ch|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case TH:     fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | th|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;
                case CTH:    fprintf (dotFile, "node%p [shape=record, label = \" {%p | %d | %d | cth|{%p | %p}}\", rang = %d]\n", node, 
                                      node, node->type, node->value,node->left, node->right, rang);
                             break;

                default: break;
            }
        }
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
    assert (dotFile);
    NodeCheckForErrors (node, VALUES_FOR_ERROR);

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

bool texPrint (Node* node, const char *fileName)
{
    assert (fileName);
    NodeCheckForErrors (node, VALUES_FOR_ERROR);

    FILE *file = fopen (fileName, "w");
    if (file == NULL)
    {
        return false;
    }

    fprintf (file, "$$");
    textPrintNodes (node, file);
    fprintf (file, "$$");

    return true;
}

bool textPrintNodes (Node *node, FILE *file)
{
    assert (file);

    if (node->type == NUMBER)
    {
        fprintf (file, "%d", node->value);
    }

    if (node->type == VARIABLE)
    {
        fprintf (file, "x");
    }

    if (node->type == CONST)
    {
        if (node->value == E)
        {
            fprintf (file, "e");
        }

        if (node->value == PI)
        {
            fprintf (file, "pi");
        }
    }

    if (node->type == OPERATION)
    {
        switch (node->value)
        {
        case ADD: fprintf (file, "(");
                  textPrintNodes (node->left, file);
                  fprintf (file, "+");
                  textPrintNodes (node->right, file);
                  fprintf (file, ")");
                  break;
        case SUB: fprintf (file, "(");
                  textPrintNodes (node->left, file);
                  fprintf (file, "-");
                  textPrintNodes (node->right, file);
                  fprintf (file, ")");
                  break;
        case MUL: textPrintNodes (node->left, file);
                  fprintf (file, "\\cdot ");
                  textPrintNodes (node->right, file);
                  break;
        case DIV: fprintf (file, "\\frac{");
                  textPrintNodes (node->left, file);
                  fprintf (file, "}{");
                  textPrintNodes (node->right, file);
                  fprintf (file, "}");
                  break;
        case POW: fprintf (file, "{");
                  textPrintNodes (node->left, file);
                  fprintf (file, "}^{");
                  textPrintNodes (node->right, file);
                  fprintf (file, "}");
                  break;
        case EXP_FUN: fprintf (file, "{");
                  textPrintNodes (node->left, file);
                  fprintf (file, "}^{");
                  textPrintNodes (node->right, file);
                  fprintf (file, "}");
                  break;
        case EXP: fprintf (file, "e^{");
                  textPrintNodes (node->right, file);
                  fprintf (file, "}");
                  break;
        case LOG: if (node->left->type == NUMBER && node->left->value == E)
                  {
                    fprintf (file, "\\ln{");
                    textPrintNodes (node->right, file);
                    fprintf (file, "}");
                    break;
                  }

                  if (node->left->type == NUMBER && node->left->value == 10)
                  {
                    fprintf (file, "\\lg{");
                    textPrintNodes (node->right, file);
                    fprintf (file, "}");
                    break;
                  }

                  fprintf (file, "\\log_{");
                  textPrintNodes (node->left, file);
                  fprintf (file, "}{");
                  textPrintNodes (node->right, file);
                  fprintf (file, "}");
                  break;
        
        default:
            break;
        }
    }

    if (node->type == TRIG_OPERATION)
    {
        switch (node->value)
        {
        case SIN:    fprintf (file, "sin (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case COS:    fprintf (file, "cos (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case TG:     fprintf (file, "tg (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case CTG:    fprintf (file, "ctg (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        case ARCSIN: fprintf (file, "arcsin (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        case ARCCOS: fprintf (file, "arccos (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        case ARCTG:  fprintf (file, "arctg (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        case ARCCTG: fprintf (file, "arcctg (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        case SH:     fprintf (file, "sh (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case CH:     fprintf (file, "ch (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case TH:     fprintf (file, "th (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;
     
        case CTH:    fprintf (file, "cth (");
                     textPrintNodes (node->left, file);
                     fprintf (file, ")");
                     break;

        default: break;
        }
    }

    return true;
}