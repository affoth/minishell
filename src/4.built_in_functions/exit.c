/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:28:07 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/11 19:29:56 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if a string is a number
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
    char **args = NULL; // Initialize args to NULL to safely check later

    // Ensure shell and command list are not NULL
    if (!shell || !cmd)
    {
        //ft_printf("Error: shell or command is NULL.\n");
        return 1; // Return 1 for internal error
    }

    args = cmd->args; // Arguments are stored in the args array

    // Check if the command arguments are valid
    if (args)
    {
        if (args[0])
        {
            if (!is_number(args[0]))
            {
                //ft_printf("exit: %s: numeric argument required\n", args[0]);
                //shell->exit_status = 255; // Default exit code for numeric argument error
                return 255; // Return 255 for numeric argument error
            }
            else if (args[1]) // Check for too many arguments
            {
                //ft_printf("exit: too many arguments\n");
                //shell->exit_status = 1; // Set exit status to 1 for error
                return 1; // Return 1 for too many arguments error
            }
            else
            {
                shell->exit_status = ft_atoi(args[0]);
                return shell->exit_status; // Return the exit code from atoi
            }
        }
    }
    else
    {
        exit(shell->exit_status); // Exit with the current exit status
    }

    // Exit the shell
    //ft_printf("Exiting shell with exit code %d\n", shell->exit_status);
    return shell->exit_status; // Return the exit status
}
