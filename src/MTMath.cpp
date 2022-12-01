#include "../include/MTMath.hpp"
#include "../include/MTString.hpp"
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MT_DEPENDENT_VAR 'X'
#define MT_IMPLICIT_NEG_CHECK ((*str == MT_DEPENDENT_VAR) || (*str == 'T') || (*str == 'S') || (*str == 'C') || (*str == 'A') || (*str == 'L') || (isdigit(*str)) || (*str == '('))

void pushOperator(linkedList *, linkedList *, node *, node *);

/// @brief Converts a null terminated string that represents a mathmatical function
/// @brief (with x being the independent variable) to a postfix expression on a linkedList
/// @param str Null terminated string to convert to postfix stack
/// @param postfix Stack containing postfix expression
/// @return Error code
int32_t infixToPostfix(char *str, linkedList *postfix)
{
	linkedList opStack;

	node tempNode;
	node opNode;
	node prevNode;

	prevNode.type = UINT8_MAX;

	bool strStart = true;
	bool testPass = false;
	uint32_t varLength = 0; // Track the last index of a variable type in the postfix stack
	short numIndex = 0;
	int8_t stCmp;

	char numBuff[32];

	strToUpper(str);

	while (*str != '\0')
	{
		testPass = false;

		if (isspace(*str)) // Skip space
		{
			str++;
			continue;
		}

		// Check if the current character is an operator, and get the operator if it is
		if ((opNode = getOperator(str, &str)).type == MT_FUNCTION)
		{
			// Check for implicit multiplication by -1
			if (((int)opNode.key == MT_FUN_SUB) && MT_IMPLICIT_NEG_CHECK)
			{
				if (postfix->head == nullptr)
				{
					// Push -1 constant onto the stack
					pushHead(postfix, createNode(-1.0l, MT_CONSTANT, 0, 0));

					// Set the operator node as multiplication
					opNode.key = MT_FUN_MUL;
					opNode.type = MT_FUNCTION;
					opNode.precedence = 2;
					opNode.associative = MT_LR_ASSOCIATIVE;
				}
				else if (prevNode.type == MT_FUNCTION)
				{
					// Push -1 constant onto the stack
					pushHead(postfix, createNode(-1.0l, MT_CONSTANT, 0, 0));

					// Set the operator node as multiplication
					opNode.key = MT_FUN_MUL;
					opNode.type = MT_FUNCTION;
					opNode.precedence = 2;
					opNode.associative = MT_LR_ASSOCIATIVE;
				}
			}
			// Implicit multiplication of function by variable or constant
			else if ((opNode.precedence == 4) && ((prevNode.type == MT_CONSTANT) || (prevNode.type == MT_VARIABLE) || ((prevNode.type == MT_CHAR) && ((int)prevNode.key == ')'))))
			{
				// Set the operator node as multiplication
				prevNode.key = MT_FUN_MUL;
				prevNode.type = MT_FUNCTION;
				prevNode.precedence = 2;
				prevNode.associative = MT_LR_ASSOCIATIVE;

				pushOperator(postfix, &opStack, &prevNode, &tempNode);
			}

			pushOperator(postfix, &opStack, &opNode, &tempNode);

			cpyNodeData(&prevNode, &opNode); // Set the previous node
		}												// End of operation parsing

		// Extract number from the string
		while (isdigit(*str) || (*str == '.'))
		{
			numBuff[numIndex] = *str;
			numIndex++;
			str++;
		}

		numBuff[numIndex] = '\0'; // Null terminate number
		if (numBuff[0] != '\0')	  // Number was read
		{
			// Implicit multiplication of digit by variable or end parentheses
			if (prevNode.type == MT_VARIABLE || ((prevNode.type == MT_CHAR) && ((int)prevNode.key == ')')))
			{
				// Set operation as multiplication
				opNode.key = MT_FUN_MUL;
				opNode.type = MT_FUNCTION;
				opNode.precedence = 2;
				opNode.associative = MT_LR_ASSOCIATIVE;

				// Push multiplication onto the stack
				pushOperator(postfix, &opStack, &opNode, &tempNode);
			}

			pushHead(postfix, createNode(strtold(numBuff, nullptr), MT_CONSTANT, 0, 0)); // Push Number node

			numIndex = 0;									// Reset number index
			cpyNodeData(&prevNode, postfix->head); // Set previous node
		}

		if (*str == 'X') // Push variable onto the stack
		{
			if ((prevNode.type == MT_CONSTANT) || (prevNode.type == MT_VARIABLE) || ((prevNode.type == MT_CHAR) && ((int)prevNode.key == ')')))
			{
				// Set operation as multiplication
				opNode.key = MT_FUN_MUL;
				opNode.type = MT_FUNCTION;
				opNode.precedence = 2;
				opNode.associative = MT_LR_ASSOCIATIVE;

				// Push multiplication onto the stack
				pushOperator(postfix, &opStack, &opNode, &tempNode);
			}

			pushHead(postfix, createNode(0, MT_VARIABLE, 0, 0)); // Push variable onto postfix stack
			varLength = postfix->size;									  // Set variable length size
			str++;															  // Next char in string

			cpyNodeData(&prevNode, postfix->head); // Set the previous node
		}

		if (*str == '(') // Store ( on the operator stack
		{
			// Implicit multiplication of start group by constant,variable, or another group
			if (prevNode.type == MT_CONSTANT || prevNode.type == MT_VARIABLE || ((prevNode.type == MT_CHAR) && ((int)prevNode.key == ')')))
			{
				// Set operation as multiplication
				opNode.key = MT_FUN_MUL;
				opNode.type = MT_FUNCTION;
				opNode.precedence = 2;
				opNode.associative = MT_LR_ASSOCIATIVE;

				// Push multiplication onto the stack
				pushOperator(postfix, &opStack, &opNode, &tempNode);
			}

			pushHead(&opStack, createNode('(', MT_CHAR, INT8_MIN, 0));

			cpyNodeData(&prevNode, opStack.head); // Set the previous node
			str++;
		}
		else if (*str == ')') // ')' has been found, search for '(' on the operator stack
		{
			popHead(&opStack, &tempNode); // Initial pop

			// Search for '(' while pushing all nodes from the operator stack onto the postfix stack
			while ((((char)tempNode.key) != '(') && (tempNode.type != MT_CHAR))
			{
				pushHead(postfix, tempNode);
				popHead(&opStack, &tempNode);
				if (isEmpty(&opStack))
					break;
			}
			// End of group was found, valid input
			if ((((char)tempNode.key) == '(') && (tempNode.type == MT_CHAR))
				;

			// Set the previous node as the ending group symbol
			tempNode.key = ')';
			tempNode.type = MT_CHAR;
			tempNode.precedence = INT8_MIN;

			cpyNodeData(&prevNode, &tempNode); // Set the previous node
			str++;
		}
	} // End of string parsing loop

	// Push the reset of the operators onto the postfix stack
	if (!isEmpty(&opStack))
		while (opStack.head != nullptr)
		{
			popHead(&opStack, &tempNode);
			pushHead(postfix, tempNode);
		}

	return varLength; // Return the last index of a variable on the postfix stack
} // infixToPostfix();

void optimizePostfix() {}

/// @brief Create a structure that must be passed into the evaluatePostfix() function
/// @brief to evaluate a postfix expression
/// @param eval Struct with necessary data to compute postfix expression
/// @param postfix Postfix expression to be executed
/// @param varLength Index of the last variable on the
/// @param var Variable to evaluate postfix expression at
void createEvalData(evalData *eval, linkedList *postfix, int32_t varLength, long double var)
{
	eval->postfix = postfix;
	eval->varLength = varLength;
	eval->var = var;
} // createEvalData();

/// @brief Evaluates a postfix expression from a evalData data struct that contains said
/// @brief postfix expression
/// @param eval Data type (usually created from createEvalData())
/// @return Long double value that results from solving the postfix expression
long double evaluatePostfix(evalData *eval)
{
	node *iterNode = eval->postfix->tail; // Track current number/operation

	// Replace variables with the input variable
	for (int64_t i = 0; i < eval->varLength; i++) // Only check bound that has variables
	{
		if (iterNode->type == MT_VARIABLE)
		{
			iterNode->key = eval->var;
		}
		iterNode = iterNode->prevLink;
	}

	// Calculate result
	iterNode = eval->postfix->tail;
	while (iterNode != nullptr)
	{
		if ((iterNode->type == MT_CONSTANT) || (iterNode->type == MT_VARIABLE))
		{
			pushHead(&eval->resStack, *iterNode);
		}
		else // Type must be function, execute said function
		{
			switch ((int)iterNode->key)
			{
			case MT_FUN_ADD:
				popHead(&eval->resStack, &eval->tempOp2);
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key += eval->tempOp2.key;

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_SUB:
				popHead(&eval->resStack, &eval->tempOp2);
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key -= eval->tempOp2.key;

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_MUL:
				popHead(&eval->resStack, &eval->tempOp2);
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key *= eval->tempOp2.key;

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_DIV:
				popHead(&eval->resStack, &eval->tempOp2);
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key /= eval->tempOp2.key;

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_POW:
				popHead(&eval->resStack, &eval->tempOp2);
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = powl(eval->tempOp1.key, eval->tempOp2.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_SIN:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = sin(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_COS:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = cos(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_TAN:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = tan(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_SINH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = sinh(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_COSH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = cosh(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_TANH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = tanh(eval->tempOp1.key);

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_CSC:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / sin(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_SEC:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / cos(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_COT:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / tan(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCSIN:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (asin(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCCOS:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (acos(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCTAN:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (atan(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCCSC:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / asin(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCSEC:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / acos(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_ARCCOT:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / atan(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_CSCH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / sinh(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_SECH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / cosh(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_COTH:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (1.0l / tanh(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			case MT_FUN_LN:
				popHead(&eval->resStack, &eval->tempOp1);

				eval->tempOp1.key = (logl(eval->tempOp1.key));

				pushHead(&eval->resStack, eval->tempOp1);
				break;
			}
		}

		// printf("%lf\n", (double)eval->resStack.head->key);

		iterNode = iterNode->prevLink; // Get next number/operation
	}

	return eval->resStack.head->key; // Return the result of the calculation
} // evaluatePostfix();

/// @brief Starts from the first char pointed to ARG0 (if nothing no first match is)
/// @brief then an error is returned
/// @param str string to find the operator in (starts searching at the first index)
/// @param str The first index of the operator MUST be passed
/// @param end Pointer to the end of the operator
/// @return // Node data containing type of operator, precedent, and associative
node getOperator(char *str, char **end)
{
	switch (*str)
	{
	case '+':
		*end = ++str;
		return createNode(MT_FUN_ADD, MT_FUNCTION, 1, MT_LR_ASSOCIATIVE);
		break;
	case '-':
		*end = ++str;
		return createNode(MT_FUN_SUB, MT_FUNCTION, 1, MT_LR_ASSOCIATIVE);
		break;
	case '*':
		*end = ++str;
		return createNode(MT_FUN_MUL, MT_FUNCTION, 2, MT_LR_ASSOCIATIVE);
		break;
	case '/':
		*end = ++str;
		return createNode(MT_FUN_DIV, MT_FUNCTION, 2, MT_LR_ASSOCIATIVE);
		break;
	case '^':
		*end = ++str;
		return createNode(MT_FUN_POW, MT_FUNCTION, 3, MT_RL_ASSOCIATIVE);
		break;
	case 'S':
		if (strncmp(str, "SINH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 4
			return createNode(MT_FUN_SINH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "SECH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 4
			return createNode(MT_FUN_SECH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "SIN", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_SIN, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "SEC", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_SEC, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		break;
	case 'C':
		if (strncmp(str, "COSH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 4
			return createNode(MT_FUN_COSH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "CSCH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 4
			return createNode(MT_FUN_CSCH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "COTH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 4
			return createNode(MT_FUN_COTH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "COS", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_COS, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "CSC", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_CSC, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "COT", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_COT, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		break;
	case 'A':
		if (strncmp(str, "ARCSIN", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCSIN, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "ARCCOS", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCCOS, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "ARCTAN", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCTAN, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "ARCCSC", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCCSC, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "ARCSEC", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCSEC, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "ARCCOT", 6) == 0)
		{
			*end = (str + 6); // Move forwards 6
			return createNode(MT_FUN_ARCCOT, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		break;
	case 'T':
		if (strncmp(str, "TANH", 4) == 0)
		{
			*end = (str + 4); // Move forwards 3
			return createNode(MT_FUN_TANH, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		else if (strncmp(str, "TAN", 3) == 0)
		{
			*end = (str + 3); // Move forwards 3
			return createNode(MT_FUN_TAN, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		break;
	case 'L':
		if (strncmp(str, "LN", 2) == 0)
		{
			*end = (str + 2); // Move forwards 2
			return createNode(MT_FUN_LN, MT_FUNCTION, 4, MT_RL_ASSOCIATIVE);
		}
		break;
	default:
		return createNode(0, 0, INT8_MIN, 0);
		;
		break;
	}
	return createNode(0, 0, INT8_MIN, 0);
} // getOperator();

void printPostfixList(const linkedList *list)
{
	node *iterNode = list->tail;
	while (iterNode != nullptr)
	{
		switch ((int)iterNode->type)
		{
		case MT_CONSTANT:
			printf("%lf ", (double)iterNode->key);
			break;
		case MT_VARIABLE:
			printf("X ");
		case MT_FUNCTION:
			switch ((int)iterNode->key)
			{
			case MT_FUN_ADD:
				printf("+ ");
				break;
			case MT_FUN_SUB:
				printf("- ");
				break;
			case MT_FUN_MUL:
				printf("* ");
				break;
			case MT_FUN_DIV:
				printf("/ ");
				break;
			case MT_FUN_POW:
				printf("^ ");
				break;
			case MT_FUN_SIN:
				printf("sin ");
				break;
			case MT_FUN_COS:
				printf("cos ");
				break;
			case MT_FUN_TAN:
				printf("tan ");
				break;
			case MT_FUN_SINH:
				printf("sinh ");
				break;
			case MT_FUN_COSH:
				printf("cosh ");
				break;
			case MT_FUN_TANH:
				printf("tanh ");
				break;
			case MT_FUN_CSC:
				printf("csc ");
				break;
			case MT_FUN_SEC:
				printf("sec ");
				break;
			case MT_FUN_COT:
				printf("cot ");
				break;
			case MT_FUN_ARCSIN:
				printf("arcsin ");
				break;
			case MT_FUN_ARCCOS:
				printf("arccos ");
				break;
			case MT_FUN_ARCTAN:
				printf("arctan ");
				break;
			case MT_FUN_ARCCSC:
				printf("arccsc ");
				break;
			case MT_FUN_ARCSEC:
				printf("arcsec ");
				break;
			case MT_FUN_ARCCOT:
				printf("arccot ");
				break;
			case MT_FUN_CSCH:
				printf("csch ");
				break;
			case MT_FUN_SECH:
				printf("sech ");
				break;
			case MT_FUN_COTH:
				printf("coth ");
				break;
			case MT_FUN_LN:
				printf("ln ");
				break;
			} // Function switch
			break;
		}										 // Type switch
		iterNode = iterNode->prevLink; // Get next number/operation
	}
	putchar('\n');
}

/// @brief Pushes and pops operations from the operation stack to the
/// @brief postfix stack for any defined operation.  This function is meant
/// @brief to internally handle adding operators to an equation.
/// NOTE: All data encapsulated by any argument passed into this function IS subject to change
/// @param postfix Stack that maintains list of variable(s), constant(s), and operators in postfix order
/// @param opStack Stack that holds operators according to rules of associativity and precedence
/// @param opNode Operator to that will be pushed onto the postfix/opStack
/// @param tempNode Node to hold temporary data
void pushOperator(linkedList *postfix, linkedList *opStack, node *opNode, node *tempNode)
{
	int8_t stCmp;

	if (opStack->head != nullptr) // Set precedence to the top of the operation stack
		stCmp = opStack->head->precedence;
	else // Nothing is on the stack, set precedence to lowest value
		stCmp = INT8_MIN;

	// If the incoming operator has a lower precedence
	if (opNode->precedence < stCmp)
	{
		while (opNode->precedence < stCmp) // While incoming oper has lower precedence
		{
			popHead(opStack, tempNode);	// Pop operator off of stack
			pushHead(postfix, *tempNode); // Push operator onto ret stack

			if (opStack->head != nullptr) // Set precedence to the top of the operation stack
				stCmp = opStack->head->precedence;
			else // Nothing is on the stack, set precedence to lowest value
				stCmp = INT8_MIN;
		}
	}
	// If the incoming operator has a higher precedence
	if (opNode->precedence > stCmp)
	{
		pushHead(opStack, *opNode); // Push operator on the operator stack
	}
	else if (opNode->precedence == stCmp) // Operators have equal precedent
	{
		// If the incoming operator is Left-Right associative
		if (opNode->associative == MT_LR_ASSOCIATIVE)
		{
			popHead(opStack, tempNode);	// Pop last operator from the stack
			pushHead(postfix, *tempNode); // Push popped operator onto the ret stack

			pushHead(opStack, *opNode); // Push incoming operator on the operations stack
		}
		else if (opNode->associative == MT_RL_ASSOCIATIVE) // If the incoming operator is Right-Left associative
			pushHead(opStack, *opNode);							// Push incoming operator on the operations stack
	}
}

int64_t getLDDenominator(long double)
{
	return 0.1;
}