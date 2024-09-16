/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:59:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 20:29:09 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void handle_file_redirections(t_command *cmd)
{
    if (cmd->stdin_fd != STDIN_FILENO)
    {
        if (dup2(cmd->stdin_fd, STDIN_FILENO) < 0)
        {
            perror("dup2 stdin_fd");
            exit(EXIT_FAILURE);
        }
        close(cmd->stdin_fd);
    }
    if (cmd->stdout_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout_fd");
            exit(EXIT_FAILURE);
        }
        close(cmd->stdout_fd);
    }
}

pid_t fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        if (!cmd->valid)
        {
            // If this command is invalid but part of a pipeline, don't exit the pipeline.
            // Just ensure that the command does not execute anything.
            if (cmd->next != NULL)
            {
                // Just exit with success so the pipeline continues
                exit(0);
            }
            // If it's the last command, exit with failure status
            exit(1);
        }

        setup_redirections(cmd_index, num_pipes, pipe_descriptors);
        // Close all pipe descriptors
        close_pipes(num_pipes, pipe_descriptors);
        // Apply file redirections
        handle_file_redirections(cmd);

        // Execute the command
        int status = 0;
        if (is_built_in(cmd->cmd_name))
        {
            status = exec_built_ins(shell, cmd);
        }
        else
        {
            status = execute_command(shell, cmd);
        }
        // Exit with the command's status
        exit(status);
    }
    else if (pid < 0) // Error handling for fork failure
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;  // Return the PID of the forked child process
}

int execute_commands_with_pipes(t_shell *shell, t_command *cmds_head)
{
    int num_pipes = count_pipes_cmdstruct(cmds_head);
    int pipe_descriptors[2 * num_pipes];
    int status = 0;  // To track the exit status
    setup_child_signals();

    create_pipes(num_pipes, pipe_descriptors);

    t_command *current_cmd = cmds_head;
    int cmd_index = 0;
    pid_t last_pid = -1;

    while (current_cmd)
    {
        pid_t pid = fork_and_execute_command(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes);
        if (pid < 0)
        {
            status = 1;  // If forking fails, set status to non-zero
        }

        // Store the PID of the last command
        if (current_cmd->next == NULL)
        {
            last_pid = pid;
        }

        // Close pipe ends not needed anymore
        if (cmd_index > 0)
        {
            close(pipe_descriptors[(cmd_index - 1) * 2]); // Close the read end of the previous pipe
        }
        if (current_cmd->next)
        {
            close(pipe_descriptors[cmd_index * 2 + 1]); // Close the write end of the current pipe
        }

        current_cmd = current_cmd->next;
        cmd_index++;
    }

    // Close all pipe descriptors in the parent process
    close_pipes(num_pipes, pipe_descriptors);
    // Wait for the last child process to complete
    int child_status;
    if (waitpid(last_pid, &child_status, 0) == -1)
    {
        perror("waitpid");
        status = 1;
    }
    else
    {
        if (WIFEXITED(child_status))
        {
            status = WEXITSTATUS(child_status);
        }
        else if (WIFSIGNALED(child_status))
        {
            status = 128 + WTERMSIG(child_status);
        }
    }

    // Wait for all other child processes
    while (wait(NULL) > 0)
        ;

    // Set shell->exit_status to the exit status of the last command
    shell->exit_status = status;

    return status;
}
