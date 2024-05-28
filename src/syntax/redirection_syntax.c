/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 19:40:42 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/28 21:03:28 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// check for syntax errors in redirections
int	redirection_syntax(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == REDIRECTION_OUT || tmp->type == REDIRECTION_IN || tmp->type == REDIRECTION_APPEND || tmp->type == HEREDOC)
		{
			if (!tmp->next || !tmp->prev)
			{
				ft_printf("redirection error: no arguments before or after redirection `%s'\n", tmp->arg);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
