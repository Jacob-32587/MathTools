#ifndef MT_LINKED_LIST_HPP
#define MT_LINKED_LIST_HPP

typedef struct node
{
	node *link = nullptr;
	long long data;
} node;

/// @brief This struct holds the head of a linkedList and it's size
typedef struct
{
	node *head = nullptr;
	unsigned long long size = 0;
} linkedList;

long long popHead(linkedList *);
void pushHead(linkedList *, long long);
void printList(linkedList);

#endif