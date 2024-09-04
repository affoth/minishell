/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:16:51 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/04 16:15:38 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
void execute_command(t_shell *shell, t_command *cmd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
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

        // Prepare command arguments
        int flags_count = 0;
        while (cmd->flags && cmd->flags[flags_count])
            flags_count++;

        int args_count = 0;
        while (cmd->args && cmd->args[args_count])
            args_count++;

        char **args = (char **)malloc(sizeof(char *) * (flags_count + args_count + 2));
        if (!args)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (i < flags_count)
        {
            args[i + 1] = strdup(cmd->flags[i]);
            if (!args[i + 1])
            {
                perror("strdup");
                while (i >= 0) free(args[i--]);
                free(args);
                exit(EXIT_FAILURE);
            }
            i++;
        }
        args[0] = strdup(cmd->cmd_name);
        if (!args[0])
        {
            perror("strdup");
            while (i >= 0) free(args[i--]);
            free(args);
            exit(EXIT_FAILURE);
        }

        int j = 0;
        while (j < args_count)
        {
            args[flags_count + 1 + j] = strdup(cmd->args[j]);
            if (!args[flags_count + 1 + j])
            {
                perror("strdup");
                while (i >= 0) free(args[i--]);
                while (j >= 0) free(args[flags_count + 1 + j--]);
                free(args);
                exit(EXIT_FAILURE);
            }
            j++;
        }
        args[flags_count + args_count + 1] = NULL;

        // Resolve the command path
        char *path = get_path(&shell->gc, args[0]);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", args[0]);
            int k = 0;
            while (args[k]) free(args[k++]);
            free(args);
            exit(EXIT_FAILURE);
        }

        execve(path, args, shell->env);
        perror("execve");

        int k = 0;
        while (args[k]) free(args[k++]);
        free(args);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Parent process should wait for the child process to complete
    while (wait(NULL) > 0);
}

void create_pipes(int num_pipes, int *pipe_descriptors)
{
    int i = 0;
    while (i < num_pipes)
    {
        if (pipe(pipe_descriptors + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

void setup_input_redirection(int cmd_index, int *pipe_descriptors)
{
    if (cmd_index > 0)
    {
        if (dup2(pipe_descriptors[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
    }
}

void setup_output_redirection(int cmd_index, int num_pipes, int *pipe_descriptors)
{
    if (cmd_index < num_pipes)
    {
        if (dup2(pipe_descriptors[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
    }
}

void close_pipes(int num_pipes, int *pipe_descriptors)
{
    int i = 0;
    while (i < 2 * num_pipes)
    {
        close(pipe_descriptors[i]);
        i++;
    }
}

int count_pipes_cmdstruct(t_command *cmds_head)
{
    int count = 0;
    while (cmds_head && cmds_head->next)
    {
        count++;
        cmds_head = cmds_head->next;
    }
    return count;
}

void fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        setup_input_redirection(cmd_index, pipe_descriptors);
        setup_output_redirection(cmd_index, num_pipes, pipe_descriptors);

        close_pipes(num_pipes, pipe_descriptors);

        execute_command(shell, cmd);
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
            // Execute built-in commands in the parent process
            exec_built_ins(shell);
        }
        else
        {
            fork_and_execute_command(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes);
        }

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

    close_pipes(num_pipes, pipe_descriptors);

    while (wait(NULL) > 0);
}

void execute_commands_without_pipes(t_shell *shell, t_command *cmds_head)
{
    t_command *current_cmd = cmds_head;

    while (current_cmd)
    {
        if (is_built_in(current_cmd->cmd_name))
        {
            // Execute built-in commands in the parent process
            exec_built_ins(shell);
        }
        else
        {
            // Execute external commands in child processes
            execute_command(shell, current_cmd);
        }

        current_cmd = current_cmd->next;
    }

    while (wait(NULL) > 0);
}
