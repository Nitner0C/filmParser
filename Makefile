NAME=		filmParser

SRC=		src/filmParser.c \
			src/list.c

OBJ=		$(SRC:.c=.o)

CFLAGS=		-Wall -Wextra -Werror -Iinclude/

CC=			gcc

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) -o $(NAME) $(OBJ)

clean:
			rm -f $(OBJ)

fclean:
			rm -f $(OBJ) $(NAME)

re:			fclean all
