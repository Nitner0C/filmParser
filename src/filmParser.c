/*
** filmParser.c for filmParser
** 
** Made by Corentin Foure
** Email   <corentin.foure@gmail.com>
** 
** Started on  Thu Jun 12 2014
** Last update Tue Jun 17 2014
*/

#include		<stdlib.h>
#include 		<stdio.h>
#include 		<dirent.h>
#include		<sys/types.h>
#include 		<sys/stat.h>
#include		<string.h>
#include 		<stdbool.h>
#include		<errno.h>
#include 		<fcntl.h>
#include		<unistd.h>
#include		<ctype.h>

#include		"list.h"
#include		"filmParser.h"

#define			CSVFILENAME "./films.csv"

int			removeDuplicates(t_list *list, bool remove)
{
	t_node		*tmp = list->head;
	bool 		fyi = false;
	int 		nbDuplicates = 0;

	while (tmp && tmp->next)
	{
		if (!strcmp(tmp->name, tmp->next->name))
		{
			if (fyi == false && remove == false)
			{
				fyi = true;
				puts("For your information these are the duplicates :");
			}
			if (remove == false)
			{
				printf("\t\tDuplicate : %s && %s\n", tmp->name, tmp->next->name);
				printf("\t\t\t\t%s\n\t\t\t\t%s\n", tmp->completePath, tmp->next->completePath);
			}
			if (remove == false)
				tmp = tmp->next;
			else
			{
				nbDuplicates++;
				removeOfList(list, tmp);
			}
		}
		else
			tmp = tmp->next;
	}
	return (nbDuplicates);
}



char			*myconcat(char *s1, char *s2)
{
	int			size;
	char		*toReturn;

	size = strlen(s1) + strlen(s2) + 2;
	toReturn = malloc(size * sizeof(*toReturn));
	bzero(toReturn, size);
	strcpy(toReturn, s1);
	if (toReturn[strlen(toReturn) - 1] != '/')
		strcat(toReturn, "/");
	strcat(toReturn, s2);
	return toReturn;
}

bool 			endsWith (char* base, char* str) 
{
	int 		blen = strlen(base);
	int 		slen = strlen(str);
	return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}

void			checkDir(char *refDir, int *offset, t_list *l, bool silent)
{
	DIR 		*dir;
	struct 		dirent *s;

	if ((dir = opendir(refDir)) == NULL)
	{
		if (silent == false)
			printf("%s : %s\n", strerror(errno), refDir);
		return;
	}
	while ((s = readdir(dir)) != NULL){
		char *newDir = myconcat(refDir, s->d_name);
		if (s->d_type == DT_DIR)
		{
			if (strcmp(s->d_name, ".") && strcmp(s->d_name, ".."))
				checkDir(newDir, offset, l, silent);
		}
		else{
			if (endsWith(newDir, ".avi") || endsWith(newDir, ".mkv"))
				addInList(l, s->d_name, refDir, newDir);
			(*offset)++;
		}
	}
	closedir(dir);
	free(refDir);
	return;
}

void			writeInFile(char *fileName, t_list *l, int appenFlag)
{
	FILE 		*fd = fopen(fileName, (appenFlag == true ? "a" : "w+"));

	if (fd != NULL)
	{
		t_node	*test = l->head;
		if (test)
			fprintf(fd, "%s;%s\n", "name", "path");
		while (test)
		{
			fprintf(fd, "%s;%s\n", test->name, test->path);
			test = test->next;
		}
		fprintf(fd, "%s;%i\n", "total", l->size);
		fclose(fd);
	}
}

void 			dispHowTo()
{
	puts("Usage : ./filmParser -d directory [-f fileToWrite] [-a] [-s]");
	puts("\tdirectory : is the root of where the program is going to be launched");
	puts("\tfileToWrite : is the .csv file where the films are going to be written in");
	puts("\t-a : appens to an already existing file instead of erasing all its content first");
	puts("\t-s : silent, doesn't show error message");
}

int 			main (int argc, char **argv)
{
	int 		appenFlag = 0;
	int 		nbFiles = 0;
	int 		c;
	char 		*dValue = NULL;
	char 		*fValue = NULL;
	bool 		silent = false;
	char 		*fullPathFile = NULL;
	char		*fullPathRoot = NULL;
	t_list		*list = initList();

	while ((c = getopt (argc, argv, "avsd:f:")) != -1)
		switch (c)
	{
		case 'a':
		appenFlag = 1;
		break;
		case 'd':
		dValue = optarg;
		break;
		case 'f':
		fValue = optarg;
		break;
		case 'v':
		dispHowTo();
		return EXIT_SUCCESS;
		break;
		case 's':
		silent = true;
		break;
	}
	if (dValue == NULL)
	{
		puts("option -d is compulsory, see ./filmParser -v to get more informations");
		return EXIT_FAILURE;
	}
	checkDir(strdup(dValue), &nbFiles, list, silent);
	sortList(list);
	if (silent == false)
		removeDuplicates(list, false);
	printf("Files scanned : %i, %i films found.\n", nbFiles, list->size);
	printf("Removed %d duplicates from list.", removeDuplicates(list, true));
	writeInFile((fValue == NULL ? CSVFILENAME : fValue), list, appenFlag);
	freeList(list);
	if ((fullPathFile = realpath((fValue == NULL ? CSVFILENAME : fValue), NULL)) == NULL)
	{
		perror(strerror(errno));
	}
	if ((fullPathRoot = realpath(dValue, NULL)) == NULL)
	{
		perror(strerror(errno));
	}
	printf("You can see in \"%s\" all the film \"%s\" contains.\n", (fullPathFile == NULL ? (fValue == NULL ? CSVFILENAME : fValue) : fullPathFile),
		(fullPathRoot == NULL ? dValue : fullPathRoot));
	return EXIT_SUCCESS;
}
