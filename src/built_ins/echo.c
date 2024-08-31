/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:19:49 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 13:22:23 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check for initial -n flags in the flags array
bool check_initial_n_flag(char **flags)
{
    int i = 0;
    bool suppress_newline = false;

    while (flags[i])
    {
        if (ft_strcmp(flags[i], "-n") == 0)
        {
            suppress_newline = true;
        }
        else
        {
            break;  // Stop checking when encountering a non-flag entry
        }
        i++;
    }
    return suppress_newline;
}

// Function to print echo arguments
void print_echo_arguments(char **args)
{
    for (int i = 0; args[i]; i++)
    {
        if (i > 0)
            ft_printf(" ");
        ft_printf("%s", args[i]);
    }
}

// echo built-in
void built_in_echo(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    if (!cmd || !cmd->args)
    {
        ft_printf("\n");  // Print a newline if no arguments are provided
        return;
    }

    // Extract flags and arguments from the command structure
    char **flags = cmd->flags;  // Flags are in the flags array
    char **args = cmd->args;    // Arguments are in the args array

    // Check for -n flags in the flags array
    bool suppress_newline = check_initial_n_flag(flags);

    // Print the arguments starting from the beginning
    print_echo_arguments(args);

    // Print newline if not suppressed
    if (!suppress_newline)
        ft_printf("\n");
}
