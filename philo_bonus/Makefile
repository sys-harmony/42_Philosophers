# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/14 10:12:10 by gdosch            #+#    #+#              #
#    Updated: 2026/05/15 17:44:49 by gdosch           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =			philo_bonus
CC =			cc
CFLAGS =		-Wall -Wextra -Werror# -g3 -fsanitize=address

SRC =			monitor_bonus.c \
				philo_bonus.c \
				sem_bonus.c \
				sim_bonus.c \
				utils_bonus.c

OBJ =			$(SRC:.c=.o)
HEADER =		philo_bonus.h

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
