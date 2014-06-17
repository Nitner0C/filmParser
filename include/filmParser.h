#ifndef 			__FILMPARSER_H__
# define 			__FILMPARSER_H__

bool 				endsWith (char* base, char* str);
char				*myconcat(char *s1, char *s2);
void				checkDir(char *refDir, int *offset, t_list *l, bool silent);
void 				dispHowTo();
int					removeDuplicates(t_list *list, bool remove);
void				writeInFile(char *fileName, t_list *l, int appenFlag);

#endif
