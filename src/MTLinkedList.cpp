#include "../include/MTLinkedList.hpp"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

node createNode(long double key, uint8_t type, int8_t precedence, uint8_t associative)
{
	node ret;
	ret.key = key;
	ret.type = type;
	ret.precedence = precedence;
	ret.associative = associative;
	return ret;
}

void cpyNodeData(node *dest, const node *src)
{
	dest->key = src->key;	// Copy data in the node
	dest->type = src->type; // Copy
	dest->precedence = src->precedence;
	dest->associative = src->associative;
}

/// @brief Check if a linked list has no elements
/// @param list Linked list to be check
/// @return true if the list is empty, otherwise false
bool isEmpty(const linkedList *list)
{
	if (list->head == nullptr)
		return true;
	return false;
}

/// @brief Remove the first head from the list and return it's value.
/// @brief The user is responsible for checking if the list is empty,
/// @brief otherwise undefined behavior may occur.
/// @param list List to pop the head from
/// @return Data from the popped node
void popHead(linkedList *list, node *retNode)
{
	node *link = list->head->nextLink; // Get the next node in the list
	cpyNodeData(retNode, list->head);  // Copy data for return

	free(list->head);	 // Delete the head node
	list->head = link; // Set the new head node

	if (link != nullptr)
		link->prevLink = nullptr; // Remove pointer to freed memory

	list->size--; // Decrease list size

	if (list->size == 1)
	{
		list->tail = nullptr;
	}
}

/// @brief Remove the first head from the list and return it's value.
/// @brief The user is responsible for checking if the list is empty,
/// @brief otherwise undefined behavior may occur.
/// @param list List to pop the head from
/// @return Data from the popped node
void popHeadKey(linkedList *list, long double *ret)
{
	node *link = list->head->nextLink; // Get the next node in the list
	*ret = list->head->key;

	free(list->head);	 // Delete the head node
	list->head = link; // Set the new head node

	if (link != nullptr)
		link->prevLink = nullptr; // Remove pointer to freed memory

	list->size--; // Decrease list size

	if (list->size == 1)
	{
		list->tail = nullptr;
	}
}

/// @brief Push a new node onto a linked list
/// @param list Linked list to push data onto
/// @param data Data that the push node will hold
void pushHead(linkedList *list, const node dataNode)
{
	node *insert = (node *)malloc(sizeof(node)); // Allocate new node on the heap
	cpyNodeData(insert, &dataNode);					// Set data of new node

	if (!isEmpty(list))
		list->head->prevLink = insert; // Set the previous link of the head
	if ((list->size == 1) && (list->tail == nullptr))
		list->tail = list->head;

	insert->prevLink = nullptr;
	insert->nextLink = list->head; // Set the next link of the pushed node
	list->head = insert;				 // Set the new head node of the list
	list->size++;						 // Increase size of the linked list
}

void printList(linkedList list)
{
	node *link = list.head;
	unsigned long long cnt = 0;

	while (link != nullptr)
	{
		printf("Element %d: Key = %f Type = %d\n", cnt, (double)link->key, link->type);
		link = link->nextLink;
		cnt++;
	}
}