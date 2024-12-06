#include "diff.h"
#include "dump.h"

Node *differentiateAndOptimize(Node *root, FILE *file)
{
    const char* stuki [] = {"Парни (те кто из продвы). Вы чет в край охуели там.\n",
                        "Почему мне последние 2-3 недели код показывают только челы с хуавея?\n",
                        "Я понимаю если вам просто лень на пары ходить, но блять, можно же просто зайти ко мне в комнату и показать код.\n",
                        "Если уж вы все такие ебать занятые, то хотя бы прислали бы хоть раз гитхаб.\n",
                        "Вам закрывать семестр через 2 недели, а дифференциатор НИКТО еще не показал.\n",
                        "Если не тянете нагрузку - скажите деду (и мне) или на крайний случай дерните меня, я вам просто неудов заранее въебу в таблицу и разойдемся.\n",
                        "Вы сейчас жестко подставляете меня, потому что когда пекусы не показывают код дед агрится на менторов и ему хуй объяснишь что это я за вами бегаю и прошу хоть что нибудь показать. \n",
                        "Ну не верю я блять что если вы хотя бы начали писать, у вас нет ни промежуточных версий, ни вопросов.\n",
                        "Если уж поступили на физтех и тем более пошли в продву по проге, то отвечайте пожалуйста за свой выбор\n"};
    int druki = sizeof(stuki)/sizeof(const char*);
    int stukiIndex = 0;

    NodeCheckForErrors (root, VALUES_FOR_ERROR);

    Node *diffNode = differentiate (root, root, file, stuki, druki, &stukiIndex);
    while (nodeOptimization (diffNode) != diffNode)
    {
        diffNode = nodeOptimization (diffNode);
    }

    dump (WIDE, root, diffNode);

    return diffNode;
}

Node *differentiate (Node *node, Node *root, FILE *file, const char **stuki, int druki, int *stukiIndex)
{
    NodeCheckForErrors(node, VALUES_FOR_ERROR);
    Node *diffNode = NULL;

    if (node->type == NUMBER)
    {
        diffNode = NUM_(0);
    }

    if (node->type == VARIABLE)
    {
        diffNode = NUM_(1);
    }

    if (node->type == CONST)
    {
        diffNode = NUM_(1);
    }

    if (node->type == OPERATION)
    {
        switch (node->value)
        {
            case ADD:     diffNode = ADD_(DIFF_(left), DIFF_(right));
                          break;
            case SUB:     diffNode = SUB_(DIFF_(left), DIFF_(right));
                          break;
            case MUL:     {
                            Node *dl = DIFF_(left);
                            Node *dr = DIFF_(right);
                            Node *l = COPY_SUB_TREE_(left);
                            Node *r = COPY_SUB_TREE_(right);
                            diffNode = ADD_(MUL_(dl, r), MUL_(l, dr)); 
                            break;
                          }
            case DIV:     {
                            Node *dl = DIFF_(left);
                            Node *dr = DIFF_(right);
                            Node *l = COPY_SUB_TREE_(left);
                            Node *r = COPY_SUB_TREE_(right);
                            Node *numerator = SUB_(MUL_(dl, r), MUL_(l, dr));
                            Node *rDet = COPY_SUB_TREE_(right);
                            Node *denominator = POW_(rDet, NUM_(2));
                            diffNode = DIV_(numerator, denominator);
                            break;
                          }
            case POW:     {
                            Node *ind = COPY_SUB_TREE_(right);
                            Node *base = POW_(COPY_SUB_TREE_(left), NUM_(node->right->value - 1));
                            if (node->left->type != CONST && node->left->type != NUMBER)
                            {
                                diffNode = MUL_(base, ind);
                            } else {
                                diffNode = MUL_(MUL_(base, ind), DIFF_(left));
                            }
                            break;
                          }
            case EXP_FUN: {
                            Node *base = copySubtree (node);
                            Node *ln = LOG_(E_, NUM_(node->left->value));
                            diffNode = MUL_(MUL_(base, ln), DIFF_(right));
                            break;
                          }
            case EXP:     {
                            Node *exp = copySubtree (node);
                            Node *ind = DIFF_(right);
                            diffNode = MUL_(exp, ind);
                            break;
                          }
            case LOG:     {
                            Node *num = DIV_(NUM_(1), MUL_(COPY_SUB_TREE_(right), LOG_(NUM_(E), COPY_SUB_TREE_(left))));
                            diffNode = MUL_(num, DIFF_(right));
                            break;
                          }

            default: break;
        }
    }

    if (node->type == TRIG_OPERATION)
    {
        switch (node->value)
        {
            case SIN:    {
                          diffNode = MUL_(COS_(COPY_SUB_TREE_(left)), DIFF_(left));
                          break;
                         }
            case COS:    {
                          diffNode = MUL_(MUL_(NUM_(-1), SIN_(COPY_SUB_TREE_(left))), DIFF_(left));
                          break;
                         }
            case TG:     {
                           Node *denominator = POW_(COS_(COPY_SUB_TREE_(left)), NUM_(2));
                           Node *div = DIV_ (NUM_(1), denominator);
                           diffNode = MUL_(div, DIFF_(left)); 
                           break;
                         }
            case CTG:    {
                           Node *denominator = POW_(SIN_(COPY_SUB_TREE_(left)), NUM_(2));
                           Node *div = SUB_(NUM_(0), (DIV_(NUM_(1), denominator)));
                           diffNode = MUL_(div, DIFF_(left));
                           break;
                         }
            case ARCSIN: {
                           Node *base = SUB_(NUM_(1), (POW_(COPY_SUB_TREE_(left), NUM_(2))));
                           Node *denominator = POW_(base, DIV_(NUM_(1), NUM_(2)));
                           diffNode = MUL_(DIV_(NUM_(1), denominator), DIFF_(left));
                           break;
                         }
            case ARCCOS: {
                           Node *base = SUB_(NUM_(1), (POW_(COPY_SUB_TREE_(left), NUM_(2))));
                           Node *denominator = POW_(base, DIV_(NUM_(1), NUM_(2)));
                           diffNode = MUL_(SUB_(NUM_(0), DIV_(NUM_(1), denominator)), DIFF_(left));
                           break;
                         }
            case ARCTG:  {
                           Node *base = ADD_(NUM_(1), (POW_(COPY_SUB_TREE_(left), NUM_(2))));
                           diffNode = MUL_(DIV_(NUM_(1), base), DIFF_(left));
                           break;
                         }
            case ARCCTG: {
                           Node *base = ADD_(NUM_(1), (POW_(COPY_SUB_TREE_(left), NUM_(2))));
                           diffNode = MUL_(SUB_(NUM_(0), DIV_(NUM_(1), base)), DIFF_(left));
                           break;
                         }
            case SH:     diffNode = MUL_(CH_(COPY_SUB_TREE_(left)), DIFF_(left));
                         break;
            case CH:     diffNode = MUL_(SH_(COPY_SUB_TREE_(left)), DIFF_(left));
                         break;
            case TH:     {
                           Node *denominator = POW_(CH_(COPY_SUB_TREE_(left)), NUM_(2));
                           Node *div = DIV_ (NUM_(1), denominator);
                           diffNode = MUL_(div, DIFF_(left));
                           break; 
                         }
            case CTH:    {
                           Node *denominator = POW_(SH_(COPY_SUB_TREE_(left)), NUM_(2));
                           Node *div = SUB_(NUM_(0), (DIV_(NUM_(1), denominator)));
                           diffNode = MUL_(div, DIFF_(left));
                           break;
                         }

            default: break;
        }
    }

    if (node != root)
    {
        printFunc (file, node, diffNode, stuki, druki, stukiIndex);
    }

    return diffNode;
}

Node *copySubtree (Node *node)
{
    NodeCheckForErrors(node, VALUES_FOR_ERROR);

    Node *cl = NULL;
    Node *cr = NULL;

    if (node->left != NULL)
    {
        cl = COPY_SUB_TREE_(left);
    }

    if (node->right != NULL)
    {
        cr = COPY_SUB_TREE_(right);
    }

    Node *nodeCopy = nodeCtor (node->type, node->value, cl, cr);

    return nodeCopy; 
}

bool printFunc (FILE *file, Node *node, Node *diffNode, const char **stuki, int druki, int *stukiIndex)
{
    assert (file);
    assert (node);
    assert (diffNode);

    if (diffNode->type != NUMBER && diffNode->type != CONST)
    {
        fprintf (file, "%s", stuki[*stukiIndex]);
        (*stukiIndex)++;
        if (*stukiIndex == druki)
        {
            *stukiIndex = 0;
        }
        
        fprintf (file, "$$(");
        textPrintNodes (node, file);
        fprintf (file, ")' =");
        textPrintNodes (diffNode, file);
        fprintf (file, "$$");
    }

    return true;
}

bool makeTexFile (const char *fileName, Node *node)
{
    assert (fileName);
    assert (node);

    FILE *file = fopen (fileName, "w");
    if (file == NULL)
    {
        return false;
    }

    fprintf (file, "\\documentclass[a4paper, 12pt]{article}\n\\usepackage{cmap}\n\\usepackage[T2A]\
                    {fontenc}\n\\usepackage[utf8]{inputenc}\n\\usepackage[english, russian]{babel}\n\\usepackage\
                    {amsmath}\n\\usepackage{amsfonts}\n\\usepackage{graphicx}\n\\usepackage{pgfplots}\n\\usepackage\
                    {indentfirst}\n\\usepackage{longtable}\n\\graphicspath{{pictures/}}\n\\DeclareGraphicsExtensions\
                    {.pdf,.png,.jpg}\n\\usepackage{natbib}\n\\usepackage{mathrsfs}\n\\usepackage[europeanresistors, \
                    americaninductors]{circuitikz}\n\\author{Анна Болотова}\n\\date{Ноябрь 2024}\n\\title{Отл 1}\n\\begin\
                    {document}\n\\maketitle\n\\newpage\n\\maketitle\n\\section{Первый}\n");

    Node *diffNode = differentiateAndOptimize(node, file);
    if (diffNode == NULL)
    {
        return false;
    }

    fprintf (file, "\\section{Второй}Далее вы вынуждены сохранять спокойное молчание. Здесь осталась несложная математическая\
                     часть. Т.к. $(x)'=1$, делаем несложный вывод, что\n");
    fprintf (file, "$$(");
    textPrintNodes (node, file);
    fprintf (file, ")' =");
    textPrintNodes (diffNode, file);
    fprintf (file, "$$\n");

    nodeDtor (diffNode);

    fprintf (file, "\\section{Третий}\nОдни плюсы\n");
    fprintf (file,"\n\\end{document}\n");

    return true;
}