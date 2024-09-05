/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_no_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:58:43 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/05 15:20:52 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Setup redirections for commands without pipes
void execute_command_without_pipes(t_shell *shell, t_command *cmd)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO); // Save original stdout

    if (cmd->stdin_fd != STDIN_FILENO)
    {
        if (dup2(cmd->stdin_fd, STDIN_FILENO) < 0)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        close(cmd->stdin_fd);
    }

    if (cmd->stdout_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
        close(cmd->stdout_fd);
    }

    // Perform the command execution
    if (is_built_in(cmd->cmd_name))
    {
        exec_built_ins(shell);
    }
    else
    {
        execute_command(shell, cmd);
    }

    // After the command execution, restore stdout and stdin
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}