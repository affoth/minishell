/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:18:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 14:09:30 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to execute a single command
void execute_command(t_shell *shell, t_command *cmd)
{
    // Check if the command is a built-in
    if (is_built_in(cmd->cmd_name))
    {
        exec_built_ins(shell);
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process

        // Handle input redirection
        if (cmd->stdin_fd != STDIN_FILENO)
        {
            if (dup2(cmd->stdin_fd, STDIN_FILENO) < 0)
            {
                perror("dup2 stdin");
                exit(EXIT_FAILURE);
            }
            close(cmd->stdin_fd);
        }

        // Handle output redirection
        if (cmd->stdout_fd != STDOUT_FILENO)
        {
            if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
            {
                perror("dup2 stdout");
                exit(EXIT_FAILURE);
            }
            close(cmd->stdout_fd);
        }

        // Prepare arguments for execve
        int flags_count = 0;
        while (cmd->flags && cmd->flags[flags_count]) {
            flags_count++;
        }
        
        int args_count = 0;
        while (cmd->args && cmd->args[args_count]) {
            args_count++;
        }
        
        char **args = (char **)malloc(sizeof(char *) * (flags_count + args_count + 2)); // +2 for cmd_name and NULL
        if (!args)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Set the command name
        args[0] = strdup(cmd->cmd_name);
        if (!args[0])
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }

        // Copy the flags
        for (int i = 0; i < flags_count; i++)
        {
            args[i + 1] = strdup(cmd->flags[i]);
            if (!args[i + 1])
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }

        // Copy the arguments
        for (int i = 0; i < args_count; i++)
        {
            args[flags_count + 1 + i] = strdup(cmd->args[i]);
            if (!args[flags_count + 1 + i])
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
        }
        args[flags_count + args_count + 1] = NULL;

        // Get the path of the command
        char *path = get_path(&shell->gc, args[0]);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", args[0]);
            exit(EXIT_FAILURE);
        }

        execve(path, args, shell->env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // Parent process: no need to handle pipes here; handled in the caller function
}

// Function to execute commands with piping
void execute_commands_with_pipes(t_shell *shell, t_command *cmds_head)
{
    t_command *current_cmd = cmds_head;
    int pipe_fds[2];
    int prev_fd = -1;

    while (current_cmd)
    {
        if (current_cmd->next)
        {
            // Create a pipe for the next command
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Fork the process
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (current_cmd->next)
            {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            // Close all pipe file descriptors in the child
            close(pipe_fds[0]);

            execute_command(shell, current_cmd);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Parent process
        if (prev_fd != -1)
        {
            close(prev_fd);
        }
        if (current_cmd->next)
        {
            close(pipe_fds[1]);
            prev_fd = pipe_fds[0];
        }

        current_cmd = current_cmd->next;
    }

    // Close the last file descriptor
    if (prev_fd != -1)
    {
        close(prev_fd);
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}

// Function to execute commands without pipes
void execute_commands_without_pipes(t_shell *shell, t_command *cmds_head)
{
    t_command *current_cmd = cmds_head;

    while (current_cmd)
    {
        execute_command(shell, current_cmd);
        current_cmd = current_cmd->next;
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}
