#include "diff.h"
#include "dump.h"

#define druki sizeof(stuki)/sizeof(const char*)
const char* stuki [] = {"Парни (те кто из продвы). Вы чет в край охуели там.\n",
                        "Почему мне последние 2-3 недели код показывают только челы с хуавея?\n",
                        "Я понимаю если вам просто лень на пары ходить, но блять, можно же просто зайти ко мне в комнату и показать код.\n",
                        "Если уж вы все такие ебать занятые, то хотя бы прислали бы хоть раз гитхаб.\n",
                        "Вам закрывать семестр через 2 недели, а дифференциатор НИКТО еще не показал.\n",
                        "Если не тянете нагрузку - скажите деду (и мне) или на крайний случай дерните меня, я вам просто неудов заранее въебу в таблицу и разойдемся.\n",
                        "Вы сейчас жестко подставляете меня, потому что когда пекусы не показывают код дед агрится на менторов и ему хуй объяснишь что это я за вами бегаю и прошу хоть что нибудь показать. \n",
                        "Ну не верю я блять что если вы хотя бы начали писать, у вас нет ни промежуточных версий, ни вопросов.\n",
                        "Если уж поступили на физтех и тем более пошли в продву по проге, то отвечайте пожалуйста за свой выбор\n"};
int index = 0;

Node *differentiateAndOptimize(Node *root, FILE *file)
{
    NodeCheckForErrors (root, VALUES_FOR_ERROR);

    Node *diffNode = differentiate (root, root, file); // TODO домножить на производную икс там, где забыла
    diffNode = nodeOptimization (diffNode); // TODO научить его дроби отнимать

    dump (WIDE, root, diffNode); // TODO разбить ответ на буквы

    return diffNode;
}

Node *differentiate (Node *node, Node *root, FILE *file)
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
            case ADD:     diffNode = ADD_(DIFF(left), DIFF(right));
                          break;
            case SUB:     diffNode = SUB_(DIFF(left), DIFF(right));
                          break;
            case MUL:     {
                            Node *dl = DIFF(left);
                            Node *dr = DIFF(right);
                            Node *l = COPY(left);
                            Node *r = COPY(right);
                            diffNode = ADD_(MUL_(dl, r), MUL_(l, dr)); 
                            break;
                          }
            case DIV:     {
                            Node *dl = DIFF(left);
                            Node *dr = DIFF(right);
                            Node *l = COPY(left);
                            Node *r = COPY(right);
                            Node *numerator = SUB_(MUL_(dl, r), MUL_(l, dr));
                            Node *rDet = COPY(right);
                            Node *denominator = POW_(rDet, NUM_(2));
                            diffNode = DIV_(numerator, denominator);
                            break;
                          }
            case POW:     {
                            Node *ind = COPY(right);
                            Node *base = POW_(X_, NUM_(node->right->value - 1));
                            diffNode = MUL_(base, ind);
                            break;
                          }
            case EXP_FUN: {
                            Node *base = copySubtree (node);
                            Node *ln = LOG_(NUM_(E), NUM_(node->left->value));
                            diffNode = MUL_(base, ln);
                            break;
                          }
            case EXP:     {
                            Node *exp = copySubtree (node);
                            Node *ind = DIFF(right);
                            diffNode = MUL_(exp, ind);
                            break;
                          }
            case LOG:     {
                            Node *num = DIV_(NUM_(1), MUL_(COPY(right), LOG_(NUM_(E), COPY(left))));
                            diffNode = MUL_(num, DIFF(right));
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
                          diffNode = MUL_(COS_(COPY(left)), DIFF(left));
                          break;
                         }
            case COS:    {
                          diffNode = MUL_(MUL_(NUM_(-1), SIN_(COPY(left))), DIFF(left));
                          break;
                         }
            case TG:     {
                           Node *denominator = POW_(COS_(COPY(left)), NUM_(2));
                           Node *div = DIV_ (NUM_(1), denominator);
                           diffNode = MUL_(div, DIFF(left)); 
                           break;
                         }
            case CTG:    {
                           Node *denominator = POW_(SIN_(COPY(left)), NUM_(2));
                           Node *div = SUB_(NUM_(0), (DIV_(NUM_(1), denominator)));
                           diffNode = MUL_(div, DIFF(left));
                           break;
                         }
            case ARCSIN: {
                           Node *base = SUB_(NUM_(1), (POW_(COPY(left), NUM_(2))));
                           Node *denominator = POW_(base, NUM_(1/2));
                           diffNode = DIV_(NUM_(1), denominator);
                           break;
                         }
            case ARCCOS: {
                           Node *base = SUB_(NUM_(1), (POW_(COPY(left), NUM_(2))));
                           Node *denominator = POW_(base, NUM_(1/2));
                           diffNode = SUB_(NUM_(0), DIV_(NUM_(1), denominator));
                           break;
                         }
            case ARCTG:  {
                           Node *base = ADD_(NUM_(1), (POW_(COPY(left), NUM_(2))));
                           diffNode = DIV_(NUM_(1), base);
                           break;
                         }
            case ARCCTG: {
                           Node *base = ADD_(NUM_(1), (POW_(COPY(left), NUM_(2))));
                           diffNode = SUB_(NUM_(0), DIV_(NUM_(1), base));
                           break;
                         }
            case SH:     diffNode = MUL_(CH_(COPY(left)), DIFF(left));
                         break;
            case CH:     diffNode = MUL_(SH_(COPY(left)), DIFF(left));
                         break;
            case TH:     {
                           Node *denominator = POW_(CH_(COPY(left)), NUM_(2));
                           Node *div = DIV_ (NUM_(1), denominator);
                           diffNode = MUL_(div, DIFF(left));
                           break; 
                         }
            case CTH:    {
                           Node *denominator = POW_(SH_(COPY(left)), NUM_(2));
                           Node *div = SUB_(NUM_(0), (DIV_(NUM_(1), denominator)));
                           diffNode = MUL_(div, DIFF(left));
                           break;
                         }

            default: break;
        }
    }

    if (node != root)
    {
        printFunc (file, node, diffNode);
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
        cl = COPY(left);
    }

    if (node->right != NULL)
    {
        cr = COPY(right);
    }

    Node *nodeCopy = nodeCtor (node->type, node->value, cl, cr);

    return nodeCopy; 
}

// TODO split into functions and move into separate file
Node *nodeOptimization (Node *node)
{
    NodeCheckForErrors (node, VALUES_FOR_ERROR);

    if (node->left != NULL)
    {
        node->left = nodeOptimization (node->left);
    }

    if (node->right != NULL)
    {
        node->right = nodeOptimization (node->right);
    }

    if (node->type != OPERATION)
    {
        return copySubtree(node);
    }

    switch (node->value)
    {

    case ADD:
    case SUB: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *sum = NUM_(node->left->value + node->right->value);
                    FREE_OLD_BRANCHES;
                    return sum;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    Node *cl = copySubtree(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                if (node->value == ADD)
                {
                    if (node->left->type == NUMBER && node->left->value == 0)
                    {
                        Node *cr = copySubtree(node->right);
                        FREE_OLD_BRANCHES;
                        return cr;
                    }
                } else {
                    if (node->left->type == NUMBER && node->left->value == 0)
                    {
                        Node *cr = MUL_(NUM_(-1), copySubtree(node->right));
                        FREE_OLD_BRANCHES;
                        return cr;
                    }
                }
                break;
              }
    case MUL: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *mul = NUM_(node->left->value * node->right->value);
                    FREE_OLD_BRANCHES;
                    return mul;
                }

                if (node->left->type == NUMBER && node->left->value == 0 || node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(0);
                }

                if (node->left->type == NUMBER && node->left->value == 1)
                {
                    Node *cr = copySubtree(node->right);
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copySubtree(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                break;
              }
    case DIV: { 
                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(0);
                }

                break;
              }
    case POW: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *num = NUM_(pow(node->left->value,node->right->value));
                    FREE_OLD_BRANCHES;
                    return num;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(1);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copySubtree(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                if (node->right->type == NUMBER && node->right->value < 0)
                {
                    Node *div = DIV_(NUM_(1), POW_(copySubtree(node->left), NUM_(- node->right->value)));
                    FREE_OLD_BRANCHES;
                    return div;
                }

                break;
              }
    case EXP: { 
                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return NUM_(0);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    FREE_OLD_BRANCHES;
                    return E_;
                }

                break;
              }

    case LOG:
    case EXP_FUN:
    default:
        return copySubtree(node);
    } 

    return copySubtree(node);
}

bool printFunc (FILE *file, Node *node, Node *diffNode)
{
    assert (file);
    assert (node);
    assert (diffNode);

    if (diffNode->type != NUMBER && diffNode->type != CONST)
    {
        fprintf (file, "%s", stuki[index]);
        index++;
        if (index == druki)
        {
            index = 0;
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

    fprintf (file, "Далее вы вынуждены сохранять спокойное молчание. Здесь осталась несложная математическая\
                     часть. Т.к. $(x)'=1$, делаем несложный вывод, что\n");
    fprintf (file, "$$(");
    textPrintNodes (node, file);
    fprintf (file, ")' =");
    textPrintNodes (diffNode, file);
    fprintf (file, "$$\n");

    nodeDtor (diffNode);

    fprintf (file, "\\section{Второй}\nВо-первых, назрел вполне законный вопрос, а где собстенно стипа? У меня как минимум кончился гель для стирки, и прям как раз на балансе стиралки осталось 6 рублей. Более того, у меня кончается гель для душа и кондиционер для волос (!), ну слава богу, сука, хоть средства для мытья посуды дохуя, буду с чистых тарелок есть, слава богу. Так блять, я еще купила морковки, чтобы сделать себе лавашиков с ней (вам это может не понравиться, но мне лично нравится), а это ебаная морковка оказалась НЕВКУСНОЙ, СУКА, ЕБАНАЯ РОССИЯ, ДАЖЕ СУКА ЕБАНУЮ МОРКОВКУ НЕ МОЖЕТ НОРМАЛЬНУЮ ПРОИЗВЕСТИ. В общем испортила мне эта хуйня все мои трапезы на ближайшую неделю, потому что лавашей я сделала дохуя и есть их надо, а то что я еще буду есть. Короче надо еще не забыть купить новую бутылку для воды, потому что старой я уже не доверяю и не пользуюсь. В общем жизнь крайне тяжелая, состояние нестабильное и тд.\n");
    fprintf (file,"\n\\end{document}\n");

    return true;
}