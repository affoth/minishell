/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:51:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/14 03:07:41 by mokutucu         ###   ########.fr       */
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

// Function to print echo arguments without quote processing
static void print_echo_arguments(t_gc *gc, char **args, bool skip_n_flag)
{
    if (!args)
        return;  // Return early if args is NULL

    int i = 0;

    // Skip the -n flag if present
    if (skip_n_flag && ft_strcmp(args[i], "-n") == 0)
    {
        i++;
    }

    while (args[i])
    {
        char *arg = ft_shell_strdup(gc, args[i]);

        if (arg)
        {
            write(STDOUT_FILENO, arg, ft_strlen(arg));
            // Free the duplicated argument as it's no longer needed
            // Note: Assuming that ft_gc_malloc handles GC for the arg
        }

        if (args[i + 1])
        {
            write(STDOUT_FILENO, " ", 1);  // Add a space between arguments
        }
        i++;
    }
}

// Echo built-in command
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

    // Print the arguments without quote processing
    print_echo_arguments(&shell->gc, cmd->args, suppress_newline);

    // Print newline if not suppressed
    if (!suppress_newline)
        write(STDOUT_FILENO, "\n", 1);

    return 0;  // Success code
}
