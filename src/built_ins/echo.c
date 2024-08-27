/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:28:26 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/28 00:05:39 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Check if the initial flags are -n
bool check_initial_n_flag(t_arg **args_head)
{
    t_arg *tmp;
    bool suppress_newline;

    suppress_newline = false;
    tmp = *args_head;
    while (tmp && ft_strcmp(tmp->arg, "-n") == 0)
    {
        suppress_newline = true;
        tmp = tmp->next;
    }
    *args_head = tmp;  // Update args_head to skip the -n flags
    return suppress_newline;
}

// Print the arguments, preserving spacing
void print_echo_arguments(t_arg *args_head)
{
    t_arg *tmp = args_head;
    bool first_arg = true;

    while (tmp)
    {
        if (!first_arg)
            ft_printf(" ");
        ft_printf("%s", tmp->arg);
        first_arg = false;
        tmp = tmp->next;
    }
}

// echo built-in
void built_in_echo(t_arg *args_head)
{
    bool suppress_newline;

    // Check for -n flags and adjust args_head
    suppress_newline = check_initial_n_flag(&args_head);

    // Print the arguments
    print_echo_arguments(args_head);

    // Print newline if not suppressed
    if (!suppress_newline)
        ft_printf("\n");
}