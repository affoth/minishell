/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:09:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 21:12:58 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* int	ft_isoperator(t_TokenType type)
{
	return (type == PIPE
		|| type == REDIRECTION_OUT
		|| type == REDIRECTION_IN
		|| type == REDIRECTION_APPEND
		|| type == HEREDOC);
} */

// Check list for syntax errors and return 1 if there is an error
int	syntax_checker(t_arg *head)
{
	if (!head)
	{
		return (1);
	}
	if (pipe_syntax(head))
	{
		return (1);
	}
	if (redirection_syntax(head))
	{
		return (1);
	}
	return (0);
}
