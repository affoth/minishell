/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:18:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 15:39:28 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
char	*get_path(t_gc *gc, char *cmd)
{
	char *path;
	char *path_env;
	char **path_split;
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
	path_split = ft_shell_split(gc, path_env, ':');
	i = 0;
	while (path_split[i])
	{
		// Construct the full path directly
		path = ft_shell_strjoin(gc, path_split[i], "/");
		path = ft_shell_strjoin(gc, path, cmd);
		if (access(path, F_OK | X_OK) == 0)
		{
			return (path);
		}
		i++;
	}
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

// Function to fork and execve
void execve_args(t_shell *shell)
{
    pid_t pid;
    char *path;
    char **args;
    int i;
    int argc;
    t_arg *args_head = shell->cmds_head->args_head;

    argc = count_arguments(args_head);
    i = 0;
    args = (char **)ft_gc_malloc(&shell->gc, (sizeof(char *) * (argc + 1)));
    while (args_head)
    {
        args[i] = ft_shell_strdup(&shell->gc, remove_quotes(&shell->gc, args_head->arg));
        args_head = args_head->next;
        i++;
    }
    args[i] = NULL;
    path = get_path(&shell->gc, args[0]);
    printf("Resolved path: %s\n", path);  // Print resolved path
    if (!path)
    {
		write(STDERR_FILENO, args[0], strlen(args[0]));
	    return;
    }
    pid = fork();
    if (pid == 0)
    {
        execve(path, args, shell->env);
        perror("Execution failed");
        _exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("Fork failed");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}