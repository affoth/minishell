/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:51:15 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 20:53:07 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to remove quotes from a string
char *remove_quotes(t_gc *gc, const char *str)
{
    char *result;
    int i;
    int j;

    result = (char *)ft_gc_malloc(gc, (strlen(str) + 1));
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
    return result;
}

// Get path for execve
char *get_path(t_gc *gc, char *cmd)
{
    char *path;
    char *path_env;
    char **path_split;
    int i;

    if (!cmd)
    {
        printf("in get_path: NO COMMAND FOUND\n");
        return NULL;
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
            return path;
        }
        i++;
    }
    return NULL;
}

