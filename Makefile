# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 15:07:05 by mokutucu          #+#    #+#              #
#    Updated: 2024/05/13 15:13:30 by mokutucu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
LIBDIR	= libs/libft/libft.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
RM		= rm -rf
SRC_DIR = src

SRCS	= src/main.c
OBJS	= $(SRCS:.c=.o)

all:	$(NAME)

$(NAME): $(OBJS) $(LIBDIR)
	$(CC) $(OBJS) $(LIBDIR) -o $(NAME) $(CFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(LIBDIR):
	@make -C libs/libft/
	@echo "compiled libft.a\n"

clean:
	$(RM) $(OBJS)
	@make clean -C libs/libft/

fclean: clean
	$(RM) $(NAME)
	@make fclean -C libs/libft/

re: fclean all

.PHONY: all clean fclean re
