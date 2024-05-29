/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:50:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/29 16:55:58 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Malloc with garbage collector
void *ft_gc_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	ft_gc_add(ptr);
	return (ptr);
}

// Add pointer to garbage collector
void ft_gc_add(void *ptr)
{
	t_garbage *new;

	new = malloc(sizeof(t_garbage));
	if (!new)
		return ;
	new->ptr = ptr;
	new->next = g_head;
	g_head = new;
}
