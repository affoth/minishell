/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:18:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/31 22:18:21 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *ft_shell_strjoin(char *s1, char *s2)
{
	char *newstr;
	int i;
	int j;

	i = 0;
	j = 0;
	newstr = (char *)ft_gc_malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	while (s1[i])
		newstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
		newstr[j++] = s2[i++];
	newstr[j] = '\0';
	return (newstr);
}

// get path for execve
char *get_path(char *cmd)
{
	char *path;
	char *path_env;
	char **path_split;
	int i;

	path_env = getenv("PATH");
	path_split = ft_shell_split(path_env, ':');
	i = 0;
	while (path_split[i])
	{
		path = ft_shell_strjoin(path_split[i], "/");
		path = ft_shell_strjoin(path, cmd);
		if (access(path, F_OK) == 0)
		{
			ft_printf("path: %s\n", path);
			return (path);
		}
		i++;
	}
	return (NULL);
}

// fork and execve
void execve_args(t_arg *args_head)
{
	pid_t pid;
	char *path;
	char **args;
	int i;

	i = 0;
	args = (char **)ft_gc_malloc(sizeof(char *) * (ft_lstsize((t_list *)args_head) + 1));
	while (args_head)
	{
		args[i] = args_head->arg;
		args_head = args_head->next;
		i++;
	}
	args[i] = NULL;
	path = get_path(args[0]);
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
