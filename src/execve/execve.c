/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:18:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/22 16:33:27 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void free_split(char **split)
{
    char **tmp = split;

    while (*tmp)
    {
        free(*tmp);
        tmp++;
    }
    free(split);
}

// Function to remove quotes from a string
char	*remove_quotes(t_gc *gc, const char *str)
{
	char *result;
	int i;
	int	j;

	result = (char *)ft_gc_malloc(gc, (strlen(str) + 1));
	if (!result)
	{
		perror("malloc");
	}

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	return result;
}

// get path for execve
char *get_path(t_gc *gc, char *cmd)
{
    char *path_env;
    char **path_split;
    char *full_path;
    int i;

    if (!cmd)
    {
        printf("in get_path: NO COMMAND FOUND\n");
        return (NULL);
    }
    // Check if cmd is already an absolute path
    if (access(cmd, F_OK | X_OK) == 0)
    {
        return ft_shell_strdup(gc, cmd);
    }
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_split = ft_shell_split(gc, path_env, ':');
    if (!path_split)
        return (NULL);

    i = 0;
    while (path_split[i])
    {
        full_path = ft_shell_strjoin(gc, path_split[i], "/");
        if (!full_path)
        {
            free_split(path_split);
            return (NULL);
        }
        char *temp_path = ft_shell_strjoin(gc, full_path, cmd);
        free(full_path);
        if (!temp_path)
        {
            free_split(path_split);
            return (NULL);
        }
        if (access(temp_path, F_OK | X_OK) == 0)
        {
            free_split(path_split);
            return temp_path;
        }
        free(temp_path);
        i++;
    }
    free_split(path_split);
    return (NULL);
}


// Function to count the number of arguments in args_head
int	count_arguments(t_arg *args_head)
{
	int count = 0;
	while (args_head) {
		count++;
		args_head = args_head->next;
	}
	return count;
}

void execve_args(t_gc *gc, t_arg *args_head)
{
    pid_t pid;
    char *path;
    char **args;
    int argc;
    int fd[2];
    int in_fd = STDIN_FILENO;

    while (args_head)
    {
        // Find the end of the current command
        t_arg *cmd_end = args_head;
        while (cmd_end && cmd_end->type != PIPE)
            cmd_end = cmd_end->next;

        // Allocate arguments
        argc = count_arguments(args_head);
        args = (char **)ft_gc_malloc(gc, (argc + 1) * sizeof(char *));
        int i = 0;
        while (args_head && args_head != cmd_end)
        {
            args[i++] = ft_shell_strdup(gc, remove_quotes(gc, args_head->arg));
            args_head = args_head->next;
        }
        args[i] = NULL;

        // Get path and execute
        path = get_path(gc, args[0]);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", args[0]);
            free(args);
            continue;
        }

        if (cmd_end && cmd_end->type == PIPE)
        {
            pipe(fd);
        }

        pid = fork();
        if (pid == 0)
        {
            if (in_fd != STDIN_FILENO)
                dup2(in_fd, STDIN_FILENO);
            if (cmd_end && cmd_end->type == PIPE)
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);

            execve(path, args, environ);
            perror("execve");
            exit(1);
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd[1]);
            if (cmd_end && cmd_end->type == PIPE)
                in_fd = fd[0];
        }

        // Move to the next command
        if (cmd_end)
            args_head = cmd_end->next;
        free(path);
        while (i > 0)
            free(args[--i]);
        free(args);
    }
}
