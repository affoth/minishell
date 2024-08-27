# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 15:07:05 by mokutucu          #+#    #+#              #
#    Updated: 2024/08/27 21:29:03 by mokutucu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell

LIBDIR  = libs/libft/libft.a
OBJDIR  = obj/
CC      = cc

CFLAGS  = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

RM      = rm -rf

SRC_DIR = src
SRCS    = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all:    $(NAME)

$(NAME): $(OBJS) $(LIBDIR)
	$(CC) $(OBJS) $(LIBDIR) -o $(NAME) $(CFLAGS) $(LDFLAGS)



$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBDIR):
	@make -s -C libs/libft/
	@echo "compiled libft.a\n"

clean:
	@rm -rf $(OBJDIR)
	@make clean -C libs/libft/

fclean: clean
	$(RM) $(NAME)
	@make fclean -C libs/libft/

re: fclean all

.PHONY: all clean fclean re
