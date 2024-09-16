/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_no_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:58:43 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 22:27:24 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int execute_command_without_pipes(t_shell *shell, t_command *cmd)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO); // Save original stdout
    int status = 0;

    // Handle stdin redirection
    if (cmd->stdin_fd != STDIN_FILENO)
    {
        if (dup2(cmd->stdin_fd, STDIN_FILENO) < 0)
        {
            perror("dup2 stdin");
            return EXIT_FAILURE; // Return failure status
        }
        close(cmd->stdin_fd);
    }

    // Handle stdout redirection
    if (cmd->stdout_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout");
            return EXIT_FAILURE; // Return failure status
        }
        close(cmd->stdout_fd);
    }

    // Perform the command execution
    if (is_built_in(cmd->cmd_name))
    {
        status = exec_built_ins(shell, cmd);
    }
    else
    {
        status = execute_command_no_pipes(shell, cmd);
        if (status == -1)
        {
            status = 126; // Command found but not executable
        }
    }

    // Restore stdout and stdin
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);

    return status; // Return the exit status
}

