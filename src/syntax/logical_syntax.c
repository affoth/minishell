/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_syntax.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:05:51 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/28 21:03:23 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	logical_syntax(t_arg	*head)
{
	t_arg	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == AND || tmp->type == OR)
		{
			if (!tmp->next || !tmp->prev)
			{
				ft_printf("logical error: no arguments before or after logical operator `%s'\n", tmp->arg);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
