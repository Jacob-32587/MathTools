#include "../include/MTLinkedList.hpp"
#include "malloc.h"
#include "stdio.h"

/// @brief Check if a linked list has no elements
/// @param list Linked list to be check
/// @return true if the list is empty, otherwise false
bool isEmpty(linkedList list)
{
	if (list.head == nullptr)
		return true;
	return false;
}

/// @brief Remove the first head from the list and return it's value.
/// @brief The user is responsible for checking if the list is empty,
/// @brief otherwise undefined behavior may occur.
/// @param list List to pop the head from
/// @return Data from the popped node
long long popHead(linkedList *list)
{
	node *link = list->head->link; // Get the next node in the list
	long long data = list->head->data;

	free(list->head);	 // Delete the head node
	list->head = link; // Set the new head node

	list->size--; // Decrease list size

	return data;
}

/// @brief Push a new node onto a linked list
/// @param list Linked list to push data onto
/// @param data Data that the push node will hold
void pushHead(linkedList *list, long long data)
{
	node *insert = (node *)malloc(sizeof(node)); // Allocate new node on the heap
	insert->data = data;									// Set data of new node

	insert->link = list->head; // Set the link of the pushed node
	list->head = insert;			// Set the new head node
	list->size++;					// Increase size of the linked list
}

void printList(linkedList list)
{
	node *link = list.head;
	unsigned long long cnt = 0;

	while (link != nullptr)
	{
		printf("Element %d: %d\n", cnt, link->data);
		link = link->link;
		cnt++;
	}
}