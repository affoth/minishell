/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:36:49 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 16:03:21 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if the argument is a redirection operator
bool is_redirection(const char *arg)
{
    return (strcmp(arg, ">") == 0 || strcmp(arg, ">>") == 0 ||
            strcmp(arg, "<") == 0 || strcmp(arg, "<<") == 0);
}

// Function to check if the argument is a file (specifically a .txt file)
bool is_file(const char *arg)
{
    // Check if the argument ends with ".txt"
    size_t len = strlen(arg);
    return (len > 4 && strcmp(arg + len - 4, ".txt") == 0);
}

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

// Function to print echo arguments up to the first redirection or file
void print_echo_arguments_until_redirection_or_file(char **args)
{
    int i = 0;
    bool after_redirection = false;

    while (args[i])
    {
        if (is_redirection(args[i]) || is_file(args[i]))
        {
            after_redirection = true;
        }
        if (!after_redirection)
        {
            if (i > 0)
                ft_printf(" ");
            ft_printf("%s", args[i]);
        }

        i++;
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

    // Print the arguments up to the first file
    print_echo_arguments_until_redirection_or_file(args);

    // Print newline if not suppressed
    if (!suppress_newline)
        ft_printf("\n");
}
