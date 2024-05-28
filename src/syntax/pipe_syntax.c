/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:55:31 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/28 21:03:20 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int pipe_syntax(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (!tmp->next || !tmp->prev)
			{
				ft_printf("pipe error: no arguments before or after pipe %s'\n", tmp->arg);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
