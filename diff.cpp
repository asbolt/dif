#include "diff.h"
#include "dump.h"

/*const char* stuki [] = {"Заметим, что:\n",
                            "Мы сейчас не будем решать задачу, мы ее просто...Решим:\n",
                            "Какое добро и будущее??\n",
                            "Ну эта хуйня объективно учит только нестандартно спать:\n",
                            "Никогда не понимала людей, которые говорят, что:\n",
                            "Слава богу нам хорошо преподавали математику и мы сами смогли посчитать это заявление хуйней ебной:\n",
                            "Здесь осталась несложная математическая часть. Нужно просто доказать, что вот эта фигня с какашками это вот это:\n",
                            "Это равно...большому числу. Оно тут не поместится.\n",
                            "Да, и ЧТО это такое:\n",
                            "Сейчас вы вынуждены сохранять спокойное молчание:\n",
                            "Давайте без взаимных оскорблений:\n",
                            "На простом языке это называется ответ. Но сложном языке это называется как-то по-сложному\n",
                            "Как же заебали челы, которые ходят во корпусам в толстовках взлет, сборной Москвы и тд. В чем прикол в них ходить? Ладно толстовки, которые даются за достижения, их понты можно понять. А те, которые ходят в толстовках взлета, которые раздали на смене, на которую брали всех подряд? Причем это касается не только корпусов, в прошлом году, когда я был в 11 классе и ездил на олимпиады, история была такой же. Носите лучше толстовки ФПМИ, они красивее.\n",
                            "Т.к. $(x)'= 1$:\n"};*/
int index = 0;
//#define druki sizeof(stuki)/sizeof(char)
const char* stuki [] = {"Парни (те кто из продвы). Вы чет в край охуели там.\n",
                        "Почему мне последние 2-3 недели код показывают только челы с хуавея?\n",
                        "Я понимаю если вам просто лень на пары ходить, но блять, можно же просто зайти ко мне в комнату и показать код.\n",
                        "Если уж вы все такие ебать занятые, то хотя бы прислали бы хоть раз гитхаб.\n",
                        "Вам закрывать семестр через 2 недели, а дифференциатор НИКТО еще не показал.\n",
                        "Если не тянете нагрузку - скажите деду (и мне) или на крайний случай дерните меня, я вам просто неудов заранее въебу в таблицу и разойдемся.\n",
                        "Вы сейчас жестко подставляете меня, потому что когда пекусы не показывают код дед агрится на менторов и ему хуй объяснишь что это я за вами бегаю и прошу хоть что нибудь показать. \n",
                        "Ну не верю я блять что если вы хотя бы начали писать, у вас нет ни промежуточных версий, ни вопросов.\n",
                        "Если уж поступили на физтех и тем более пошли в продву по проге, то отвечайте пожалуйста за свой выбор\n"};


Node *diff (Node *root, FILE *file) // TODO differentiateAndOptimize
{
    NodeCheckForErrors (root, __LINE__, __FUNCTION__, __FILE__); // TODO make macro

    Node *diffNode = makeDiff (root, root, file); // TODO домножить на производную икс там, где забыла
    diffNode = nodeOptimization (diffNode); // TODO научить его дроби отнимать

    dump (WIDE, root, diffNode);

    return diffNode;
}

Node *makeDiff (Node *node, Node *root, FILE *file) // TODO differencitate
{
    NodeCheckForErrors(node, __LINE__, __FUNCTION__, __FILE__);
    Node *diffNode = NULL;

    if (node->type == NUMBER)
    {
        diffNode = _NUM(0); // TODO NUMBER_NODE() or NUM_()
                            // Do not use names which begin with underscore
    }

    if (node->type == VARIABLE)
    {
        diffNode = _NUM(1);
    }

    if (node->type == CONST)
    {
        diffNode = _NUM(1);
    }

    if (node->type == OPERATION)
    {
        switch (node->value)
        {
            // TODO DIFF(left), COPY(right)
            case ADD:     diffNode = _ADD(makeDiff (node->left, root, file), makeDiff (node->right, root, file));
                          break;
            case SUB:     diffNode = _SUB(makeDiff (node->left, root, file), makeDiff (node->right, root, file));
                          break;
            case MUL:     {
                            Node *dl = makeDiff (node->left, root, file);
                            Node *dr = makeDiff (node->right, root, file);
                            Node *l = copy (node->left);
                            Node *r = copy (node->right);
                            diffNode = _ADD(_MUL(dl, r), _MUL(l, dr)); 
                            break;
                          }
            case DIV:     {
                            Node *dl = makeDiff (node->left, root, file);
                            Node *dr = makeDiff (node->right, root, file);
                            Node *l = copy (node->left);
                            Node *r = copy (node->right);
                            Node *numerator = _SUB(_MUL(dl, r), _MUL(l, dr));
                            Node *rDet = copy (node->right);
                            Node *denominator = _POW(rDet, _NUM(2));
                            diffNode = _DIV(numerator, denominator);
                            break;
                          }
            case POW:     {
                            Node *ind = copy (node->right);
                            Node *base = _POW(_X, _NUM(node->right->value - 1));
                            diffNode = _MUL(base, ind);
                            break;
                          }
            case EXP_FUN: {
                            Node *base = copy (node);
                            Node *ln = _LOG(_NUM(E), _NUM(node->left->value));
                            diffNode = _MUL(base, ln);
                            break;
                          }
            case EXP:     {
                            Node *exp = copy (node);
                            Node *ind = makeDiff (node->right, root, file);
                            diffNode = _MUL(exp, ind);
                            break;
                          }
            case LOG:     {
                            Node *num = _DIV(_NUM(1), _MUL(copy (node->right), _LOG(_NUM(E), copy (node->left))));
                            diffNode = _MUL(num, makeDiff(node->right, root, file));
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
                          diffNode = _MUL(_COS(copy (node->left)), makeDiff (node->left, root, file));
                          break;
                         }
            case COS:    {
                          diffNode = _MUL(_MUL(_NUM(-1), _SIN(copy (node->left))), makeDiff (node->left, root, file));
                          break;
                         }
            case TG:     {
                           Node *denominator = _POW(_COS(copy (node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           diffNode = _MUL(div, makeDiff (node->left, root, file)); 
                           break;
                         }
            case CTG:    {
                           Node *denominator = _POW(_SIN(copy (node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           diffNode = _MUL(div, makeDiff (node->left, root, file));
                           break;
                         }
            case ARCSIN: {
                           Node *base = _SUB(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           diffNode = _DIV(_NUM(1), denominator);
                           break;
                         }
            case ARCCOS: {
                           Node *base = _SUB(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           Node *denominator = _POW(base, _NUM(1/2));
                           diffNode = _SUB(_NUM(0), _DIV(_NUM(1), denominator));
                           break;
                         }
            case ARCTG:  {
                           Node *base = _ADD(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           diffNode = _DIV(_NUM(1), base);
                           break;
                         }
            case ARCCTG: {
                           Node *base = _ADD(_NUM(1), (_POW(copy (node->left), _NUM(2))));
                           diffNode = _SUB(_NUM(0), _DIV(_NUM(1), base));
                           break;
                         }
            case SH:     diffNode = _MUL(_CH(copy (node->left)), makeDiff (node->left, root, file));
                         break;
            case CH:     diffNode = _MUL(_SH(copy (node->left)), makeDiff (node->left, root, file));
                         break;
            case TH:     {
                           Node *denominator = _POW(_CH(copy (node->left)), _NUM(2));
                           Node *div = _DIV (_NUM(1), denominator);
                           diffNode = _MUL(div, makeDiff(node->left, root, file));
                           break; 
                         }
            case CTH:    {
                           Node *denominator = _POW(_SH(copy (node->left)), _NUM(2));
                           Node *div = _SUB(_NUM(0), (_DIV(_NUM(1), denominator)));
                           diffNode = _MUL(div, makeDiff (node->left, root, file));
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

Node *copy (Node *node) // TODO copySubtree
{
    NodeCheckForErrors(node, __LINE__, __FUNCTION__, __FILE__);

    Node *cl = NULL;
    Node *cr = NULL;

    if (node->left != NULL)
    {
        cl = copy (node->left);
    }

    if (node->right != NULL)
    {
        cr = copy (node->right);
    }

    Node *nodeCopy = nodeCtor (node->type, node->value, cl, cr);

    return nodeCopy; 
}

// TODO split into functions and move into separate file
Node *nodeOptimization (Node *node)
{
    NodeCheckForErrors (node, __LINE__, __FUNCTION__, __FILE__);

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
        return copy(node);
    }

    switch (node->value)
    {

    // TODO merge cases for add and sub
    case ADD: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *sum = _NUM(node->left->value + node->right->value);
                    FREE_OLD_BRANCHES;
                    return sum;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    Node *cr = copy(node->right);
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                break;
              }
    case SUB: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *sum = _NUM(node->left->value + node->right->value);
                    FREE_OLD_BRANCHES;
                    return sum;
                }

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    Node *cr = _MUL(_NUM(-1), copy(node->right));
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                break;
              }
    case MUL: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *mul = _NUM(node->left->value * node->right->value);
                    FREE_OLD_BRANCHES;
                    return mul;
                }

                // TODO merge 2 following ifs
                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->left->type == NUMBER && node->left->value == 1)
                {
                    Node *cr = copy(node->right);
                    FREE_OLD_BRANCHES;
                    return cr;
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }
              }
    case DIV: { 
                /*if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *div = _NUM(node->left->value / node->right->value);
                    FREE_OLD_BRANCHES;
                    return div;
                }*/

                if (node->left->type == NUMBER && node->left->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                break;
              }
    case POW: { 
                if (node->left->type == NUMBER && node->right->type == NUMBER)
                {
                    Node *num = _NUM(pow(node->left->value,node->right->value));
                    FREE_OLD_BRANCHES;
                    return num;
                }

                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(1);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    Node *cl = copy(node->left);
                    FREE_OLD_BRANCHES;
                    return cl;
                }

                if (node->right->type == NUMBER && node->right->value < 0)
                {
                    Node *div = _DIV(_NUM(1), _POW(copy(node->left), _NUM(- node->right->value)));
                    FREE_OLD_BRANCHES;
                    return div;
                }

                break;
              }
    case EXP: { 
                if (node->right->type == NUMBER && node->right->value == 0)
                {
                    FREE_OLD_BRANCHES;
                    return _NUM(0);
                }

                if (node->right->type == NUMBER && node->right->value == 1)
                {
                    FREE_OLD_BRANCHES;
                    return _E;
                }

                break;
              }

    case LOG:
    case EXP_FUN:
    default:
        return copy(node);
    } 

    return copy(node);
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
        if (index == 9)
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

    Node *diffNode = diff (node, file);
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

    fprintf (file, "\\section{Второй}\nВо-первых, назрел вполне законный вопрос, а где собстенно стипа? У меня как минимум кончился гель для стирки, и прям как раз на балансе стиралки осталось 6 рублей. Более того, у меня кончается гель для душа и кондиционер для волос (!), ну слава богу, сука, хоть средства для мытья посуды дохуя, буду с чистых тарелок есть, слава богу. Так блять, я еще купила морковки, чтобы сделать себе лавашиков с ней (вам это может не понравиться, но мне лично нравится), а это ебаная морковка оказалась НЕВКУСНОЙ, СУКА, ЕБАНАЯ РОССИЯ, ДАЖЕ СУКА ЕБАНУЮ МОРКОВКУ НЕ МОЖЕТ НОРМАЛЬНУЮ ПРОИЗВЕСТИ. В общем испортила мне эта хуйня все мои трапезы на ближайшую неделю, потому что лавашей я сделала дохуя и есть их надо, а то что я еще буду есть. Короче надо еще не забыть купить новую бутылку для воды, потому что старой я уже не доверяю и не пользуюсь. В общем жизнь крайне тяжелая, состояние нестабильное и тд.\n");
    fprintf (file,"\n\\end{document}\n");

    return true;
}