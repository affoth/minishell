/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:50:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/29 19:50:56 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_garbage *g_head = NULL; // Define and initialize the global variable

// Function to allocate memory and add to garbage collector
void *ft_gc_malloc(size_t size)
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
	new_node->next = g_head;
	g_head = new_node;

	return ptr;
}

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
