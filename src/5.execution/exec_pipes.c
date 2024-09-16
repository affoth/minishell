/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:59:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 16:12:34 by afoth            ###   ########.fr       */
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
        // Close the original file descriptor if it's not standard input
        if (cmd->stdin_fd != STDIN_FILENO)
            close(cmd->stdin_fd);
    }
    if (cmd->stdout_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout_fd");
            exit(EXIT_FAILURE);
        }
        // Close the original file descriptor if it's not standard output
        if (cmd->stdout_fd != STDOUT_FILENO)
            close(cmd->stdout_fd);
    }
}
int fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes)
{
    setup_child_signals();
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        setup_redirections(cmd_index, num_pipes, pipe_descriptors);
        // Close all pipe descriptors
        close_pipes(num_pipes, pipe_descriptors);

        // Apply file redirections
        handle_file_redirections(cmd);

        // Execute the command
        if (is_built_in(cmd->cmd_name))
        {
            exec_built_ins(shell, cmd);
        }
        else
        {
            execute_command(shell, cmd);
        }
        // If exec fails
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) // Error handling for fork failure
    {
        perror("fork");
        exit(EXIT_FORK_FAILED);
    }

    return 0;  // Return 0 to indicate success in fork
}

int execute_commands_with_pipes(t_shell *shell, t_command *cmds_head)
{
    int num_pipes = count_pipes_cmdstruct(cmds_head);
    int pipe_descriptors[2 * num_pipes];
    int status = 0;  // To track the exit status

    create_pipes(num_pipes, pipe_descriptors);

    t_command *current_cmd = cmds_head;
    int cmd_index = 0;

    while (current_cmd)
    {
        if (fork_and_execute_command(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes) != 0)
        {
            status = 1;  // If forking fails, set status to non-zero
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

    // Wait for all child processes to complete
    int child_status;
    while (wait(&child_status) > 0)
    {
        if (WIFEXITED(child_status))
        {
            int exit_code = WEXITSTATUS(child_status);
            if (exit_code != 0)  // If any command failed, set status to the last non-zero exit code
            {
                status = exit_code;
            }
        }
        else if (WIFSIGNALED(child_status))
        {
            status = 128 + WTERMSIG(child_status); // Capture signal-based exit status
        }
    }

    return status;  // Return the exit status
}
