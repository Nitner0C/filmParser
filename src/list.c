/*
** filmParser.c for filmParser
** 
** Made by Corentin Foure
** Email   <corentin.foure@gmail.com>
** 
** Started on  Thu Jun 12 2014
** Last update Tue Jun 17 2014
*/

#include		<stdio.h>
#include		<stdbool.h>
#include		<stdlib.h>
#include		<string.h>

#include		"list.h"


static void		dispElem(t_node *elem, int verbose)
{
	printf("Elem");
	if (verbose == NAME || verbose == BOTH)
		printf(" [name = %s]", elem->name);
	if (verbose == PATH || verbose == BOTH)
		printf(" [path = %s]", elem->path);
	printf("\n");
	return;
}

static void		freeNode(t_node *node)
{
	if (node)
	{
		if (node->name)
			free(node->name);
		if (node->path)
			free(node->path);
		if (node->completePath)
			free(node->completePath);
		free(node);
	}
}

static void		freeAllNodes(t_node *node)
{
	if (node->next)
		freeAllNodes(node->next);
	freeNode(node);
}

void			dispList(t_list *list, int verbose)
{
	t_node		*head = list->head;

	while (head)
	{
		dispElem(head, verbose);
		head = head->next;
	}
}

void			sortList(t_list *list)
{
	bool		isSorted = false;
	t_node		*tmp = list->head;
	
	while (isSorted == false)
	{
		isSorted = true;
		tmp = list->head;
		while (tmp && tmp->next)
		{
			if (strcmp(tmp->name, tmp->next->name) > 0)
			{
				char *nameSave = tmp->name;
				char *completePathSave = tmp->completePath;
				char *pathSave = tmp->path;

				tmp->name = tmp->next->name;
				tmp->path = tmp->next->path;
				tmp->completePath = tmp->next->completePath;

				tmp->next->name = nameSave;
				tmp->next->path = pathSave;
				tmp->next->completePath = completePathSave;
				isSorted = false;
			}
			tmp = tmp->next;
		}
	}
}

void			addInList(t_list *l, char *name, char *path, char *completePath)
{
	t_node		*new = malloc(sizeof(*new));
	if (new)
	{
		new->name = strdup(name);
		new->path = strdup(path);
		new->completePath = strdup(completePath);
		new->next = l->head;
		if (l->head)
			new->id = l->head->id + 1;
		else
			new->id = 0;
		l->head = new;
		l->size += 1;
	}
	else
		fprintf(stderr, "%s\n", "Memory allocation failure");
}

void			freeList(t_list *list)
{
	if (list)
	{
		if (list->head)
		{
			freeAllNodes(list->head);
			list->head = NULL;
		}
		free(list);
		list = NULL;
	}
}

void			removeOfList(t_list *list, t_node *prevNode)
{
	if (list && prevNode)
	{
		t_node *toRemove = prevNode->next;
		prevNode->next = toRemove->next;
		list->size -= 1;
		freeNode(toRemove);
	}
}

t_list			*initList()
{
	t_list 		*list = malloc(sizeof(t_list));

	if (list)
	{
		list->head = NULL;
		list->size = 0;
	}
	return (list);
}