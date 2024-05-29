/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:51:00 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/29 16:56:02 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Free all pointers in garbage collector
void ft_gc_free(void)
{
	t_garbage *tmp;

	while (g_head)
	{
		tmp = g_head;
		g_head = g_head->next;
		free(tmp->ptr);
		free(tmp);
	}
}
