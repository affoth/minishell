/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:51:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 14:50:07 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check for initial -n flags in the flags array
bool check_initial_n_flag(char **flags)
{
    if (!flags) return false;  // Return early if flags is NULL

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

// Function to print echo arguments up to the first redirection or file
void print_echo_arguments(char **args, bool skip_n_flag)
{
    if (!args) return;  // Return early if args is NULL

    int i = 0;

    // Skip the -n flag if present
    if (skip_n_flag && ft_strcmp(args[i], "-n") == 0)
    {
        i++;
    }

    while (args[i])
    {
        write(STDOUT_FILENO, args[i], ft_strlen(args[i]));

        if (args[i + 1])
        {
            write(STDOUT_FILENO, " ", 1);  // Add a space between arguments
        }
        i++;
    }
}

// echo built-in
int built_in_echo(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    if (!cmd || !cmd->args)
    {
        write(STDOUT_FILENO, "\n", 1);  // Print a newline if no arguments are provided
        return 0;  // Success code
    }

    // Check for -n flags in the flags array
    bool suppress_newline = check_initial_n_flag(cmd->flags);

    // Print the arguments up to the first file or redirection operator
    print_echo_arguments(cmd->args, suppress_newline);

    // Print newline if not suppressed
    if (!suppress_newline)
        write(STDOUT_FILENO, "\n", 1);

    return 0;  // Success code
}
