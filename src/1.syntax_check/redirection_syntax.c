/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:59:28 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 03:19:40 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if a character is valid in a filename
int	is_valid_char(char c)
{
	if (ft_isalnum(c) || c == '.' || c == '-' || c == '_')
	{
		return (1);
	}
	return (0);
}

//check flag!!
int	redirection_input_syntax_check(t_arg *tmp)
{
	if (!tmp->next || !tmp->prev)
	{
		printf("redirection input error:"
			" no arguments before or after redirection `%s'\n", tmp->arg);
		return (1);
	}
	if (tmp->next->type != WORD && tmp->next->type != DOUBLE_QUOTED_STRING)
	{
		printf("redirection input error:"
			" no valid file name after redirection `%s'\n", tmp->arg);
		return (1);
	}
	if (tmp->prev->type != WORD)
	{
		printf("redirection input error:"
			" no valid argument before redirection `%s'\n", tmp->arg);
		return (1);
	}
	return (0);
}

//check flag!!
int	redirection_output_syntax_check(t_arg *tmp)
{
	if (!tmp->next || !tmp->prev)
	{
		printf("redirection output error:"
			" no arguments before or after redirection `%s'\n", tmp->arg);
		return (1);
	}
	if (tmp->next->type != WORD)
	{
		printf("redirection output error:"
			" no valid file name after redirection `%s'\n", tmp->arg);
		return (1);
	}
	return (0);
}

int	redirection_heredoc_syntax_check(t_arg *tmp)
{
	if (tmp->next->type != WORD)
	{
		ft_printf("redirection error:"
			" no valid file name after redirection `%s'\n", tmp->arg);
		return (1);
	}
	return (0);
}

// check for syntax errors in redirections
int	redirection_syntax(t_arg *head)
{
	t_arg	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == REDIRECTION_IN)
		{
			if (redirection_input_syntax_check(tmp))
				return (1);
		}
		if (tmp->type == REDIRECTION_OUT)
		{
			if (redirection_output_syntax_check(tmp))
				return (1);
		}
		if (tmp->type == REDIRECTION_APPEND || tmp->type == HEREDOC)
		{
			if (redirection_heredoc_syntax_check(tmp))
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
