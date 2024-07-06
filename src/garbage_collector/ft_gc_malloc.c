/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:50:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 13:08:21 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to allocate memory and add to garbage collector
void *ft_gc_malloc(t_gc *gc, size_t size)
{
	void *ptr;
	t_garbage *new_node;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	new_node = (t_garbage *)malloc(sizeof(t_garbage));
	if (!new_node)
	{
		perror("malloc failed for garbage collector node");
		free(ptr);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;

	return ptr;
}

void ft_gc_free(t_gc *gc)
{
	t_garbage *tmp;
	while (gc->head)
	{
		tmp = gc->head;
		gc->head = gc->head->next;
		free(tmp->ptr);
		free(tmp);
	}
}
