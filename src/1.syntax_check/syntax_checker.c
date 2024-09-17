/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:09:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 23:21:03 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_isoperator(TokenType type)
{
    return (type == PIPE ||
            type == REDIRECTION_OUT || type == REDIRECTION_IN ||
            type == REDIRECTION_APPEND || type == HEREDOC);
}

// Check list for syntax errors and return 1 if there is an error
int syntax_checker(t_arg *head)
{
    t_arg *tmp = head;

    if (!head)
    {
        return 1; // Empty input
    }

    while (tmp)
    {
        // Check for consecutive operators or an operator at the end
        if (ft_isoperator(tmp->type) && (tmp->next == NULL || ft_isoperator(tmp->next->type)))
        {
            printf("syntax error: same operator next to each other or next is null `%s'\n", tmp->arg);
            return 1;
        }
        tmp = tmp->next;
    }

    // Perform operator-specific syntax checks
    if (pipe_syntax(head) || redirection_syntax(head))
    {
        return 1;
    }

    return 0;
}
