/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:38:27 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/22 15:38:39 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void handle_redirections(t_gc *gc, t_arg *args_head)
{
    t_arg *current = args_head;

    while (current)
    {
        if (current->type == REDIRECTION_IN)
        {
            simple_input_redirection(gc, current, current->next);
        }
        else if (current->type == REDIRECTION_OUT)
        {
            simple_output_redirection(gc, current, current->next);
        }
        else if (current->type == REDIRECTION_APPEND)
        {
            simple_append_redirection(gc, current, current->next);
        }
        else if (current->type == HEREDOC)
        {
            heredoc(current->next->arg);
        }
        current = current->next;
    }
}
