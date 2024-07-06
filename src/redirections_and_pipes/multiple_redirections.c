/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:28:09 by afoth             #+#    #+#             */
/*   Updated: 2024/07/06 16:51:00 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*
2 -> find pipe
execute pipe
while head != NULL
    1 = 2
	2 -> find pipe
	exec pipe (1, 2)*/


/* void	multiple_redirections(t_gc *gc, t_arg *head)
{
	t_arg	*first_arg;
	t_arg	*second_arg;

	first_arg = head;
	second_arg = head;
	while (second_arg != NULL)
	{
		if (second_arg->type == REDIRECTION_IN || second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND || second_arg->type == HEREDOC || second_arg->type == PIPE)
		{
			handle_multiple_redirections_and_pipes(gc, first_arg, second_arg->next);
			first_arg = second_arg;
		}
		second_arg = second_arg->next;
	}
}

void	handle_multiple_redirections_and_pipes(t_gc *gc, t_arg *first_arg, t_arg *second_arg)
} */



void	multiple_redirections(t_arg *head)
{
	t_arg	*first_arg;
	t_arg	*second_arg;
	int	fd;

	fd = -2;
	first_arg = head;
	second_arg = head;
	while (second_arg != NULL)
	{
		if (second_arg->type == REDIRECTION_IN || second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND || second_arg->type == HEREDOC || second_arg->type == PIPE)
		{
			// if (second_arg->type == PIPE)
			// 	second_arg = multiple_pipes(first_arg, second_arg->next);
			// else
				fd = handle_multiple_redirections_and_pipes(first_arg, second_arg->next, fd);
				if (is_executable(second_arg->next))
					first_arg = second_arg->next;
		}
		second_arg = second_arg->next;
	}
}
//check with get_path if the command is executable
int	is_executable(t_arg *arg)
{
	char	*path;

	path = get_path(arg->arg);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", arg->arg);
		return (0);
	}
	return (1);
}

int	handle_multiple_redirections_and_pipes(t_arg *first_arg, t_arg *second_arg, int fd)
{
	if (second_arg->prev->type == REDIRECTION_IN)
	{
		fd = advanced_input_redirection(gc, second_arg, first_arg);
	}
	else if (second_arg->prev->type == REDIRECTION_OUT)
	{
		output_redirection(gc, first_arg, second_arg, fd);
	}
	else if (second_arg->prev->type == REDIRECTION_APPEND)
	{
		append_redirection(gc, second_arg, first_arg, fd);
	}
	else if (second_arg->prev->type == HEREDOC)
	{
		// Get the delimiter which is the next argument
        if (second_arg->next && (second_arg->next->type == WORD ||
                            second_arg->next->type == DOUBLE_QUOTED_STRING ||
                            second_arg->next->type == SINGLE_QUOTED_STRING))
        {
            printf("I GOT  CHANGED< CHECK MULTIPLE REDIRECTION\n");
		//heredoc(second_arg->next->arg);
        }
	}
	else if (second_arg->prev->type == PIPE)
	{
		//pipe_redirection(second_arg->next, first_arg, fd);
		fd = multiple_pipes(gc, first_arg, second_arg, fd);
	}
	return (fd);
}

/* int	count_pipes(t_arg *head)
{
	int	count;

	count = 0;
	while (head)
	{
		if (head->type == PIPE)
			count++;
		head = head->next;
	}
	return (count);
} */

