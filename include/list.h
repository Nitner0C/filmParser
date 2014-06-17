#ifndef 			__FILMPARSER_H__
# define 			__FILMPARSER_H__

# define			NAME 	1
# define			PATH 	2
# define			BOTH	3

typedef struct 		s_node
{
	int 			id;
	char			*name;
	char			*path;
	char			*completePath;
	struct s_node	*next;
}					t_node;

typedef struct 		s_list
{
	t_node			*head;
	unsigned int 	size;
}					t_list;

void				addInList(t_list *l, char *name, char *path, char *completePath);
void				dispList(t_list *list, int verbose);
void				sortList(t_list *list);
void				freeList(t_list *list);
void				removeOfList(t_list *list, t_node *prevNode);
t_list				*initList();

#endif