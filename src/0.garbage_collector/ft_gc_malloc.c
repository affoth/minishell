/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:50:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/10 17:29:13 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to initialize the garbage collector
void ft_gc_init(t_gc *gc, t_shell *shell)
{
    gc->head = NULL;
    gc->shell = shell;  // Set the shell pointer
}

// Function to allocate memory and add to garbage collector
void *ft_gc_malloc(t_gc *gc, size_t size)
{
    void *ptr;
    t_garbage *new_node;

    ptr = malloc(size);
    if (!ptr)
    {
        perror("malloc failed");
        gc->shell->exit_status = 1; // Set exit status to 1 for memory allocation failure
        exit(EXIT_FAILURE);
    }

    new_node = (t_garbage *)malloc(sizeof(t_garbage));
    if (!new_node)
    {
        perror("malloc failed for garbage collector node");
        free(ptr);
        gc->shell->exit_status = 1; // Set exit status to 1 for memory allocation failure
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
