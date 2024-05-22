# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 15:07:05 by mokutucu          #+#    #+#              #
#    Updated: 2024/05/22 15:57:00 by mokutucu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell

LIBDIR  = libs/libft/libft.a

CC      = cc

CFLAGS  = -Wall -Wextra -Werror
LDFLAGS = -lreadline

RM      = rm -rf

SRC_DIR = src
SRCS    = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst %.c,%.o,$(SRCS))

all:    $(NAME)

$(NAME): $(OBJS) $(LIBDIR)
	$(CC) $(OBJS) $(LIBDIR) -o $(NAME) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(LIBDIR):
	@make -s -C libs/libft/
	@echo "compiled libft.a\n"

clean:
	$(RM) $(OBJS)
	@make clean -C libs/libft/

fclean: clean
	$(RM) $(NAME)
	@make fclean -C libs/libft/

re: fclean all

.PHONY: all clean fclean re
