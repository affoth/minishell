/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:13:40 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/29 15:14:13 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//check if WORD is not empty and valid

int word_syntax(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			if (tmp->arg[0] == '\0')
			{
				ft_printf("word error: empty word\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
