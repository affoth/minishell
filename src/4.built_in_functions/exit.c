/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:21:56 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/13 18:25:43 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if a string is a valid integer number
bool is_number(const char *str)
{
    if (*str == '-' || *str == '+')
        str++;
    if (*str == '\0')
        return false; // Empty string is not a number
    while (*str)
    {
        if (!ft_isdigit(*str))
            return false;
        str++;
    }
    return true;
}

// Function to strip quotes from a string
char *strip_quotes(t_gc *gc, const char *str)
{
    size_t i = 0, j = 0;
    char *result;
    size_t len = ft_strlen(str);

    // Allocate memory for the result string
    result = (char *)ft_gc_malloc(gc, len + 1);
    if (!result)
        return NULL; // Memory allocation failed

    while (str[i])
    {
        // Skip opening quote
        if (str[i] == '\'' || str[i] == '\"')
        {
            char quote = str[i];
            i++;
            while (str[i] && str[i] != quote)
            {
                result[j++] = str[i++];
            }
            if (str[i] == quote)
                i++; // Skip closing quote
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0'; // Null-terminate the result string

    return result;
}

static void exiting_with(t_shell *shell, int code)
{
    ft_gc_free(&shell->gc); // Free all memory with garbage collector
    ft_putstr_fd("exit\n", STDERR_FILENO); // Print 'exit' message
    exit(code);
}

int built_in_exit(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    char **args = NULL;
    int code = 0;
    char *cleaned_arg = NULL;

    if (!shell || !cmd)
        return 1; // Internal error

    args = cmd->args;

    if (args && args[0])
    {
        // Handle argument with quotes
        cleaned_arg = strip_quotes(&shell->gc, args[0]);
        if (!cleaned_arg)
            return 1; // Error in memory allocation

        if (!is_number(cleaned_arg))
        {
            ft_putstr_fd("exit: ", STDERR_FILENO);
            ft_putstr_fd(args[0], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            exiting_with(shell, 2); // Exit with error code 2
            return 2; // Error code
        }
        else
        {
            code = ft_atoi(cleaned_arg);
            // Adjust code to be within 0-255 using modulo operation
            code = code % 256;

            if (code < 0)
                code += 256; // Handle negative values properly

            if (args[1])
            {
                ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
                return 1; // Error code
            }
            else
            {
                exiting_with(shell, code); // Exit with the computed code
                return code; // This line will never be reached
            }
        }
    }
    else
    {
        exiting_with(shell, 0); // Exit with default code 0
        return 0; // This line will never be reached
    }
}
