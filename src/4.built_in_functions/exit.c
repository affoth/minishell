/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:28:07 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/02 17:53:42 by mokutucu         ###   ########.fr       */
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
void built_in_exit(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    char **args = NULL; // Initialize args to NULL to safely check later
    int exit_code = 0;

    // Ensure shell and command list are not NULL
    if (!shell || !cmd)
    {
        ft_printf("Error: shell or command is NULL.\n");
        exit(EXIT_FAILURE);
    }

    args = cmd->args; // Arguments are stored in the args array

    // Check if the command arguments are valid
    if (args)
    {
        if (args[1])
        {
            if (!is_number(args[1]))
            {
                ft_printf("exit: %s: numeric argument required\n", args[1]);
                exit_code = 255; // Default exit code for numeric argument error
            }
            else if (args[2]) // Check for too many arguments
            {
                ft_printf("exit: too many arguments\n");
                shell->exit_status = 1; // Set exit status to 1 for error
                return; // Do not exit the shell
            }
            else
            {
                exit_code = ft_atoi(args[1]);
            }
        }
    }
    else
    {
        // No arguments provided, default exit code is 0
        ft_printf("exit\n");
    }
    
    // Exit the shell
    exit(exit_code);
}
