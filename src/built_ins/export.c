/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/18 18:22:47 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void built_in_export(t_arg *args_head, char ***env)
{
	t_arg *tmp;

	tmp = args_head;
	while (tmp)
	{
		if(ft_strchr(tmp->arg, '='))
		{
			
		}

		tmp = tmp->next;
	}
}
