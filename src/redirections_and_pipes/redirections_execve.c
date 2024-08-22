/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 17:19:55 by afoth             #+#    #+#             */
/*   Updated: 2024/08/22 16:33:00 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to count the number of arguments in args_head
int redirect_count_arguments(t_arg *args_head)
{
	int count = 0;
	while (args_head)
	{
		if (args_head->type == REDIRECTION_IN || args_head->type == REDIRECTION_OUT ||
			args_head->type == REDIRECTION_APPEND || args_head->type == HEREDOC ||
			args_head->type == PIPE)
			return count;
		count++;
		args_head = args_head->next;
	}
	return count;
}
void redirect_execve_args(t_gc *gc, t_arg *args_head)
{
    t_arg *tmp;
    char *path;
    char **args;
    int i;
    int argc;

    tmp = args_head;
    argc = redirect_count_arguments(tmp);
    args = (char **)ft_gc_malloc(gc, (sizeof(char *) * (argc + 1)));
    i = 0;
    while (tmp && i < argc)
    {
        if (tmp->type == WORD || tmp->type == DOUBLE_QUOTED_STRING ||
            tmp->type == SINGLE_QUOTED_STRING)
        {
            args[i] = ft_shell_strdup(gc, remove_quotes(gc, tmp->arg));
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;

    // Check if the command is a built-in
    if (is_built_in(args[0]))
    {
        exec_built_ins(gc, args_head);
        return; // Do not continue with external command execution
    }

    // For external commands
    path = get_path(gc, args[0]);
    if (!path)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        return;
    }

    // Fork a new process for external commands
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // In child process
        execve(path, args, environ);
        perror("execve");
        exit(1);
    }
    else
    {
        // In parent process
        waitpid(pid, NULL, 0);
    }
}



void execute_with_pipes(t_gc *gc, t_arg *args_head)
{
    int fd[2];
    pid_t pid;
    int in_fd = STDIN_FILENO;
    t_arg *current = args_head;
    t_arg *command_start = args_head;

    while (current)
    {
        if (current->type == PIPE || current->next == NULL)
        {
            if (current->type == PIPE)
            {
                pipe(fd);
            }
            pid = fork();

            if (pid == 0)  // Child process
            {
                dup2(in_fd, STDIN_FILENO);  // Set input from previous pipe
                if (current->type == PIPE)
                    dup2(fd[1], STDOUT_FILENO);  // Set output to next pipe

                close(fd[0]);

                // Handle any redirection in the command
                handle_redirections(gc, command_start);

                // Execute the command
                redirect_execve_args(gc, command_start);
                exit(0);
            }
            else  // Parent process
            {
                waitpid(pid, NULL, 0);
                close(fd[1]);
                in_fd = fd[0];
                command_start = current->next;  // Move to next command
            }
        }
        current = current->next;
    }
}
