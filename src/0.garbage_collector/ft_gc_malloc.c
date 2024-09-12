/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:50:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 13:26:59 by mokutucu         ###   ########.fr       */
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

void *ft_gc_realloc(t_gc *gc, void *ptr, size_t old_size, size_t new_size)
{
    // Check for valid input
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (!ptr)
    {
        return ft_gc_malloc(gc, new_size);
    }
    
    // Allocate new memory
    void *new_ptr = ft_gc_malloc(gc, new_size);
    if (!new_ptr)
        return NULL;
    
    // Copy old data to new memory
    ft_memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);

    // Free old memory
    free(ptr);

    return new_ptr;
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
