SRCS	=	$(addprefix srcs/, main.c free.c run_philo.c set_philos.c utils.c utils2.c)

OBJS	=	$(SRCS:.c=.o)

CFLAGS	=	-Wall -Wextra -Werror

NAME	=	philo

CC		=	clang

HEADER	=	-I includes

.c.o:
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $(<:.c=.o)

all:		${NAME}

${NAME}:	${OBJS}
			${CC} ${CFLAGS} ${HEADER} ${OBJS} -o ${NAME}

clean:
			rm -f ${OBJS}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
