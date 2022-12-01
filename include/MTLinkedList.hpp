#ifndef MT_LINKED_LIST_HPP
#define MT_LINKED_LIST_HPP
#include <stdint.h>

#define MT_CONSTANT 0
#define MT_VARIABLE 1
#define MT_FUNCTION 2
#define MT_CHAR 3

typedef struct node
{
	node *nextLink = nullptr;
	node *prevLink = nullptr;
	long double key = 0;
	uint8_t type = MT_CONSTANT;
	int8_t precedence = 0;
	uint8_t associative = 0;
} node;

/// @brief This struct holds the head of a linkedList and it's size
typedef struct
{
	node *head = nullptr;
	node *tail = nullptr;
	unsigned long long size = 0;
} linkedList;

node createNode(long double, uint8_t, int8_t, uint8_t);
void cpyNodeData(node *, const node *);
bool isEmpty(const linkedList *);

void popHead(linkedList *, node *);
void pushHead(linkedList *, const node);

void printList(linkedList);

#endif