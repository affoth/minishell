/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:51:15 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/20 02:06:52 by mokutucu         ###   ########.fr       */
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

char *normalize_path(t_shell *shell, char *path) {
    if (path[0] == '/' || (path[0] == '.' && (path[1] == '/' || path[1] == '.'))) {
        if (access(path, F_OK) != 0) {
            fprintf(stderr, "No such file or directory: %s\n", path);
            exit(EXIT_COMMAND_NOT_FOUND);
        }
        if (is_directory(path)) {
            exit(EXIT_PERMISSION_DENIED); // Directory check integrated
        }
        if (access(path, X_OK) != 0) {
            fprintf(stderr, "Permission denied: %s\n", path);
            exit(EXIT_PERMISSION_DENIED);
        }
        return ft_shell_strdup(&shell->gc, path);
    }
    return ft_shell_strdup(&shell->gc, path); // Return normalized path for other cases
}

char *get_path(t_shell *shell, char *cmd) {
    // Handle absolute and relative paths
    if (cmd[0] == '.' || cmd[0] == '/') {
        return normalize_path(shell, cmd);
    }

    char *path_env = ft_getenv(shell, "PATH");
    if (!path_env) {
        exit(EXIT_COMMAND_NOT_FOUND); // PATH not set
    }

    char **path_split = ft_shell_split(&shell->gc, path_env, ':');
    for (int i = 0; path_split[i]; i++) {
        char *path = ft_shell_strjoin(&shell->gc, path_split[i], "/");
        path = ft_shell_strjoin(&shell->gc, path, cmd);

        if (access(path, F_OK) == 0) {
            if (is_directory(path)) {
                exit(EXIT_PERMISSION_DENIED); // Directory check integrated
            }
            if (access(path, X_OK) == 0) {
                return path; // Found and executable
            }
            fprintf(stderr, "Permission denied: %s\n", path);
            exit(EXIT_PERMISSION_DENIED);
        }
    }
    fprintf(stderr, "Command not found: %s\n", cmd);
    exit(EXIT_COMMAND_NOT_FOUND);
}
