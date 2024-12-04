#ifndef DSL_H_
#define DSL_H_

#define NUM_(arg) nodeCtor (NUMBER, arg, NULL, NULL)

#define X_ nodeCtor (VARIABLE, X, NULL, NULL)

#define ADD_(left, right) nodeCtor (OPERATION, ADD, left, right)
#define SUB_(left, right) nodeCtor (OPERATION, SUB, left, right)
#define MUL_(left, right) nodeCtor (OPERATION, MUL, left, right)
#define DIV_(left, right) nodeCtor (OPERATION, DIV, left, right)
#define POW_(left, right) nodeCtor (OPERATION, POW, left, right)
#define EXP_FUN_(left, right) nodeCtor (OPERATION, EXP_FUN, left, right)
#define EXP_(right) nodeCtor (OPERATION, EXP, E_, right)
#define LOG_(left, right) nodeCtor (OPERATION, LOG, left, right)

#define SIN_(left) nodeCtor (TRIG_OPERATION, SIN, left, NULL)
#define COS_(left) nodeCtor (TRIG_OPERATION, COS, left, NULL)
#define TG_(left) nodeCtor (TRIG_OPERATION, TG, left, NULL)
#define CTG_(left) nodeCtor (TRIG_OPERATION, CTG, left, NULL)
#define ARCSIN_(left) nodeCtor (TRIG_OPERATION, ARCSIN, left, NULL)
#define ARCCOS_(left) nodeCtor (TRIG_OPERATION, ARCCOS, left, NULL)
#define ARCTG_(left) nodeCtor (TRIG_OPERATION, ARCTG, left, NULL)
#define ARCCTG_(left) nodeCtor (TRIG_OPERATION, ARCCTG, left, NULL)
#define SH_(left) nodeCtor (TRIG_OPERATION, SH, left, NULL)
#define CH_(left) nodeCtor (TRIG_OPERATION, CH, left, NULL)
#define TH_(left) nodeCtor (TRIG_OPERATION, TH, left, NULL)
#define CTH_(left) nodeCtor (TRIG_OPERATION, CTH, left, NULL)

#define E_ nodeCtor (CONST, E, NULL, NULL)
#define PI_ nodeCtor (CONST, PI, NULL, NULL)

#define DIFF(branch) differentiate (node->branch, root, file)
#define COPY(branch) copySubtree (node->branch)

#endif