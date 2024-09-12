/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:28:07 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 15:11:56 by mokutucu         ###   ########.fr       */
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

// Exit built-in command
int built_in_exit(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    char **args = NULL;

    // Ensure shell and command list are not NULL
    if (!shell || !cmd)
    {
        return 1; // Return 1 for internal error
    }

    args = cmd->args;

    // Check if there are arguments
    if (args && args[0])
    {
        if (!is_number(args[0]))
        {
            // Numeric argument required error
            ft_putstr_fd("exit: ", STDERR_FILENO);
            ft_putstr_fd(args[0], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            // Set the exit status for numeric argument error
            return 2; // Return the exit code
        }
        else if (args[1]) // Check for too many arguments
        {
            ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
            // Set the exit status for too many arguments error
            return 1; // Return the exit code
        }
        else
        {
            // Convert the argument to an integer
            int exit_code = ft_atoi(args[0]);
            // Normalize exit code to the range 0-255
            if (exit_code < 0)
                exit_code = 256 + (exit_code % 256); // Handle negative exit codes
            else if (exit_code > 255)
                exit_code = exit_code % 256; // Handle exit codes above 255

            return exit_code; // Return the exit code
        }
    }
    else
    {
        // No arguments, use the current exit status
        // Default exit status if no arguments
        return 0;
    }
}
