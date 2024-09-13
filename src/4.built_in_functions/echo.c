/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:51:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/13 17:43:41 by mokutucu         ###   ########.fr       */
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

/* static bool is_quote_closed(const char *str, char quote_char)
{
    // Check if there is a matching closing quote in the remaining string
    while (*str)
    {
        if (*str == quote_char)
            return true;
        str++;
    }
    return false;
} */

static char *process_quotes(t_gc *gc, const char *s)
{
    char *result = ft_shell_strdup(gc, s);
    if (!result)
        return NULL;

    char *write_ptr = result;   // Pointer for writing the result
    const char *read_ptr = s;   // Pointer for reading the input
    bool in_single_quote = false;
    bool in_double_quote = false;

    while (*read_ptr)
    {
        if (*read_ptr == '\'' && !in_double_quote)
        {
            // Toggle single quote state
            in_single_quote = !in_single_quote;
        }
        else if (*read_ptr == '\"' && !in_single_quote)
        {
            // Toggle double quote state
            in_double_quote = !in_double_quote;
        }
        else
        {
            // Copy characters to the result string if not inside quotes
            if (!in_single_quote && !in_double_quote)
            {
                *write_ptr++ = *read_ptr;
            }
            else if (in_single_quote || in_double_quote)
            {
                // Include characters inside quotes in the result
                *write_ptr++ = *read_ptr;
            }
        }
        read_ptr++;
    }
    *write_ptr = '\0';

    return result;
}


// Function to print echo arguments with proper handling of quotes
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
        char *processed_arg = process_quotes(gc, args[i]);

        if (processed_arg)
        {
            write(STDOUT_FILENO, processed_arg, ft_strlen(processed_arg));
            // Free the processed argument as it's no longer needed
            // Note: Assuming that ft_gc_malloc handles GC for the processed_arg
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

    // Print the arguments with proper handling of quotes
    print_echo_arguments(&shell->gc, cmd->args, suppress_newline);

    // Print newline if not suppressed
    if (!suppress_newline)
        write(STDOUT_FILENO, "\n", 1);

    return 0;  // Success code
}
