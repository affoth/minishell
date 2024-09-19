/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:51:15 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 17:01:09 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to remove quotes from a string
char	*remove_quotes(t_gc *gc, const char *str)
{
	char	*result;
	int		i;
	int		j;

	result = (char *)ft_gc_malloc(gc, (ft_strlen(str) + 1));
	if (!result)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
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
	return (result);
}

char	*ft_getenv(t_shell *shell, const char *name)
{
	char	**environ;
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	environ = shell->env;
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			return (&environ[i][len + 1]);
		}
		i++;
	}
	return (NULL);
}

// Get path for execve
char	*get_path(t_shell *shell, char *cmd)
{
	char	*path;
	char	*path_env;
	char	**path_split;
	int		i;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_shell_strdup(&shell->gc, cmd));
	path_env = ft_getenv(shell, "PATH");
	if (!path_env)
		return (NULL);
	path_split = ft_shell_split(&shell->gc, path_env, ':');
	i = 0;
	while (path_split[i])
	{
		path = ft_shell_strjoin(&shell->gc, path_split[i], "/");
		path = ft_shell_strjoin(&shell->gc, path, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);
}
