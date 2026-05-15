# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 11:47:52 by gdosch            #+#    #+#              #
#    Updated: 2026/05/15 18:46:14 by gdosch           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =			philo
CC =			cc
CFLAGS =		-Wall -Wextra -Werror# -g3 -fsanitize=thread

SRC =			monitor.c \
				mutex.c \
				philo.c \
				sim.c \
				utils.c

OBJ =			$(SRC:.c=.o)
HEADER =		philo.h

.PHONY:			all clean fclean re

all:			$(NAME)

%.o:			%.c $(HEADER)
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
				rm -f $(OBJ)

fclean:			clean
				rm -f $(NAME)

re:				fclean all
