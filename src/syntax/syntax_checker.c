/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:09:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/28 21:19:32 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_isoperator(TokenType type)
{
	if (type == PIPE || type == AND || type == OR ||
			type == REDIRECTION_OUT || type == REDIRECTION_IN ||
			type == REDIRECTION_APPEND || type == HEREDOC)
		return (1);
	return (0);
}

//	check list for syntax errors and return 1 if there is an error
int syntax_checker(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	while (tmp)
	{
		// check if there is an operator at the beginning or end of the list
		if (tmp == head && ft_isoperator(tmp->type))
		{
			printf("syntax error: operator at beginning or end `%s'\n", tmp->arg);
			return (1);
		}
		// check if there is an operator after another operator
		if (ft_isoperator(tmp->type) && (tmp->next == NULL || ft_isoperator(tmp->next->type)))
		{
				printf("syntax error: same operator next to eachother or next is null `%s'\n", tmp->arg);
				return (1);
		}
		// operator specific syntax checks
		if (pipe_syntax(head) || redirection_syntax(head) || logical_syntax(head))
		{
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
