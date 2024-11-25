#ifndef DSL_H_
#define DSL_H_

#define E 2.73

#define _NUM(arg) nodeCtor (NUMBER, arg, NULL, NULL)

#define _X nodeCtor (VARIABLE, X, NULL, NULL)

#define _ADD(left, right) nodeCtor (OPERATION, ADD, left, right)
#define _SUB(left, right) nodeCtor (OPERATION, SUB, left, right)
#define _MUL(left, right) nodeCtor (OPERATION, MUL, left, right)
#define _DIV(left, right) nodeCtor (OPERATION, DIV, left, right)
#define _POW(left, right) nodeCtor (OPERATION, POW, left, right)
#define _EXP_FUN(left, right) nodeCtor (OPERATION, EXP_FUN, left, right)
#define _EXP(right) nodeCtor (OPERATION, EXP, E, right)
#define _LOG(left, right) nodeCtor (OPERATION, LOG, left, right)

#define _SIN(left) nodeCtor (TRIG_OPERATION, SIN, left, NULL)
#define _COS(left) nodeCtor (TRIG_OPERATION, COS, left, NULL)
#define _TG(left) nodeCtor (TRIG_OPERATION, TG, left, NULL)
#define _CTG(left) nodeCtor (TRIG_OPERATION, CTG, left, NULL)
#define _ARCSIN(left) nodeCtor (TRIG_OPERATION, ARCSIN, left, NULL)
#define _ARCCOS(left) nodeCtor (TRIG_OPERATION, ARCCOS, left, NULL)
#define _ARCTG(left) nodeCtor (TRIG_OPERATION, ARCTG, left, NULL)
#define _ARCCTG(left) nodeCtor (TRIG_OPERATION, ARCCTG, left, NULL)
#define _SH(left) nodeCtor (TRIG_OPERATION, SH, left, NULL)
#define _CH(left) nodeCtor (TRIG_OPERATION, CH, left, NULL)
#define _TH(left) nodeCtor (TRIG_OPERATION, TH, left, NULL)
#define _CTH(left) nodeCtor (TRIG_OPERATION, CTH, left, NULL)

#endif