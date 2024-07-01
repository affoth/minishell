/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:28:09 by afoth             #+#    #+#             */
/*   Updated: 2024/07/01 15:54:33 by afoth            ###   ########.fr       */
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


/* void	multiple_redirections(t_arg *head)
{
	t_arg	*first_arg;
	t_arg	*second_arg;

	first_arg = head;
	second_arg = head;
	while (second_arg != NULL)
	{
		if (second_arg->type == REDIRECTION_IN || second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND || second_arg->type == HEREDOC || second_arg->type == PIPE)
		{

			first_arg = second_arg;
		}
		second_arg = second_arg->next;
	}
} */



void	multiple_redirections(t_arg *head)
{
	t_arg	*first_arg;
	t_arg	*second_arg;

	first_arg = head;
	second_arg = head;
	while (second_arg != NULL)
	{
		if (second_arg->type == REDIRECTION_IN || second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND || second_arg->type == HEREDOC || second_arg->type == PIPE)
		{
			// if (second_arg->type == PIPE)
			// 	second_arg = multiple_pipes(first_arg, second_arg->next);
			// else
				handle_multiple_redirections_and_pipes(first_arg, second_arg);
			first_arg = second_arg->next;
		}
		second_arg = second_arg->next;
	}
}

void	handle_multiple_redirections_and_pipes(t_arg *first_arg, t_arg *second_arg)
{
	if (second_arg->type == REDIRECTION_IN)
	{
		input_redirection(second_arg->next, first_arg);
	}
	else if (second_arg->type == REDIRECTION_OUT)
	{
		output_redirection(second_arg->next, first_arg);
	}
	else if (second_arg->type == REDIRECTION_APPEND)
	{
		append_redirection(second_arg->next, first_arg);
	}
	else if (second_arg->type == HEREDOC)
	{
		heredoc(second_arg->next, first_arg);
	}
	else if (second_arg->type == PIPE)
	{
		pipe_redirection(second_arg->next, first_arg);
	}
}



