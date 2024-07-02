/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:28:09 by afoth             #+#    #+#             */
/*   Updated: 2024/07/02 13:51:34 by mokutucu         ###   ########.fr       */
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


void	multiple_redirections(t_gc *gc, t_arg *head)
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
{
	if (second_arg->type == REDIRECTION_IN)
	{
		input_redirection(gc, second_arg, first_arg);
	}
	else if (second_arg->type == REDIRECTION_OUT)
	{
		output_redirection(gc, second_arg, first_arg);
	}
	else if (second_arg->type == REDIRECTION_APPEND)
	{
		append_redirection(gc, second_arg, first_arg);
	}
	else if (second_arg->type == HEREDOC)
	{
		// Get the delimiter which is the next argument
        if (second_arg->next && (second_arg->next->type == WORD ||
                            second_arg->next->type == DOUBLE_QUOTED_STRING ||
                            second_arg->next->type == SINGLE_QUOTED_STRING))
        {
            heredoc(second_arg->next->arg);
        }
	}
	else if (second_arg->type == PIPE)
	{
		pipe_redirection(gc, second_arg, first_arg);
	}
}


