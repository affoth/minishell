/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:32:49 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 13:26:59 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_env_len(char **env)
{
	int env_len;

	env_len= 0;
	while (env[env_len])
		env_len++;
	return env_len;
}

char *find_variable(t_gc *gc, const char *arg)
{
	size_t var_name_len = ft_strchr(arg, '=') - arg;
	return ft_shell_strndup(gc, arg, var_name_len);
}

int find_var_in_env(char **env, const char *var_name)
{
	int env_idx;
	size_t var_name_len;

	var_name_len = ft_strlen(var_name);
	env_idx = 0;
	while (env[env_idx])
	{
		if (ft_strncmp(env[env_idx], var_name, var_name_len) == 0 && env[env_idx][var_name_len] == '=')
		{
			return env_idx;
		}
		env_idx++;
	}
	return (-1);
}

char **add_env_var(t_gc *gc, char *arg, char **env, int env_len)
{
	int i;
	char **new_env;

	new_env = ft_gc_malloc(gc, (env_len + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("No memory left for new_env");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (i < env_len)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[env_len] = ft_shell_strdup(gc, arg);
	new_env[env_len + 1] = NULL;
	return new_env;
}

char **change_or_add_env_var(t_gc *gc, char *arg, char **env)
{
	int env_len;

	env_len = ft_env_len(env);

	char *var_name = find_variable(gc, arg);
	int var_idx = find_var_in_env(env, var_name);

	if (var_idx != -1)
	{
		env[var_idx] = ft_shell_strdup(gc, arg);
	}
	else
	{
		env = add_env_var(gc, arg, env, env_len);
	}
	return env;
}
