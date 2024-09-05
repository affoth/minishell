/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:59:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/05 14:59:32 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        setup_redirections(cmd_index, num_pipes, pipe_descriptors);
        close_pipes(num_pipes, pipe_descriptors);

        if (is_built_in(cmd->cmd_name))
        {
            // Execute built-in command with proper redirection
            exec_built_ins(shell);
        }
        else
        {
            execute_command(shell, cmd);
        }
        exit(EXIT_FAILURE); // Ensure to exit after executing the command
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void execute_commands_with_pipes(t_shell *shell, t_command *cmds_head)
{
    int num_pipes = count_pipes_cmdstruct(cmds_head);
    int pipe_descriptors[2 * num_pipes];

    create_pipes(num_pipes, pipe_descriptors);

    t_command *current_cmd = cmds_head;
    int cmd_index = 0;

    while (current_cmd)
    {
        if (is_built_in(current_cmd->cmd_name))
        {
            // Special case: Execute built-in command in the pipeline
            fork_and_execute_command(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes);
        }
        else
        {
            // External command; fork and execute
            fork_and_execute_command(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes);
        }

        // Close the pipe ends not needed anymore
        if (cmd_index > 0)
        {
            close(pipe_descriptors[(cmd_index - 1) * 2]);
        }
        if (current_cmd->next)
        {
            close(pipe_descriptors[cmd_index * 2 + 1]);
        }

        current_cmd = current_cmd->next;
        cmd_index++;
    }

    // Close all pipe descriptors
    close_pipes(num_pipes, pipe_descriptors);

    // Wait for all child processes to complete
    while (wait(NULL) > 0);
}
