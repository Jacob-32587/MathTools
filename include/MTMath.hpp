#ifndef MT_MATH_HPP
#define MT_MATH_HPP

#include "../include/MTLinkedList.hpp"

// Basic operators

#define MT_FUN_ADD 1
#define MT_FUN_SUB 2
#define MT_FUN_MUL 3
#define MT_FUN_DIV 4
#define MT_FUN_POW 5

// Trig Functions

#define MT_FUN_SIN 10
#define MT_FUN_COS 11
#define MT_FUN_TAN 12

#define MT_FUN_CSC 13
#define MT_FUN_SEC 14
#define MT_FUN_COT 15

#define MT_FUN_ARCSIN 16
#define MT_FUN_ARCCOS 17
#define MT_FUN_ARCTAN 18

#define MT_FUN_ARCCSC 19
#define MT_FUN_ARCSEC 20
#define MT_FUN_ARCCOT 21

#define MT_FUN_SINH 22
#define MT_FUN_COSH 23
#define MT_FUN_TANH 25

#define MT_FUN_CSCH 26
#define MT_FUN_SECH 27
#define MT_FUN_COTH 28

// Log Power Sqrt

#define MT_FUN_LN 29

#define MT_LR_ASSOCIATIVE 1
#define MT_RL_ASSOCIATIVE 2

typedef struct
{
	linkedList resStack;
	linkedList *postfix;
	node tempOp1;
	node tempOp2;
	int32_t varLength;
	long double var;
} evalData;

typedef struct
{
	long double val = 0.0l;
	int32_t denominator = 1.0l;
} exponentData;

int32_t infixToPostfix(char *, linkedList *);
void createEvalData(evalData *, linkedList *, int32_t, long double);
long double evaluatePostfix(evalData *eval);
bool isLRAssociative(short);
node getOperator(char *, char **);
void printPostfixList(const linkedList *);
int64_t getLDDenominator(long double);

#endif