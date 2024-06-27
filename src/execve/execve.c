/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:18:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/27 20:50:46 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to remove quotes from a string
char	*remove_quotes(const char *str)
{
	char *result;
	int i;
	int	j;

	result = (char *)ft_gc_malloc(strlen(str) + 1);
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
char	*get_path(char *cmd)
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
		return ft_shell_strdup(cmd);
	}
	path_env = getenv("PATH");
	path_split = ft_shell_split(path_env, ':');
	i = 0;
	while (path_split[i])
	{
		// Construct the full path directly
		path = ft_shell_strjoin(path_split[i], "/");
		path = ft_shell_strjoin(path, cmd);
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

// fork and execve
void	execve_args(t_arg *args_head)
{
	pid_t pid;
	char *path;
	char **args;
	int i;
	int argc;

	argc = count_arguments(args_head);
	i = 0;
	args = (char **)ft_gc_malloc(sizeof(char *) * (argc + 1));
	while (args_head)
	{
		args[i] = ft_shell_strdup(remove_quotes(args_head->arg));
		args_head = args_head->next;
		i++;
	}
	args[i] = NULL;
	path = get_path(args[0]);
	printf("Resolved path: %s\n", path);  // Print resolved path
	if (!path)
	{
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, NULL);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
