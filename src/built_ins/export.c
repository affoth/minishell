/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/19 17:59:38 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Add a new environment variable to the environment array
char **add_env_var(char *arg, char **env)
{
	char **new_env;
	int env_idx;
	int env_len;

	env_idx = 0;
	env_len = 0;
	while (env[env_len])
	{
		env_len++;
	}
	// (env_len + 2 for the new variable and NULL terminator)
	new_env = ft_gc_malloc((env_len + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	while (env[env_idx])
	{
		new_env[env_idx] = ft_strdup(env[env_idx]);
		env_idx++;
	}
	new_env[env_idx] = ft_strdup(arg);
	new_env[env_idx + 1] = NULL;
	return new_env;
}

// Handle the export built-in command
void built_in_export(t_arg *args_head, char ***env)
{
	t_arg *tmp;

	tmp = args_head;
	if (tmp->next == NULL)
	{
		built_in_env(*env);
		return;
	}
	tmp = tmp->next;
	while (tmp != NULL)
	{
		if (ft_strchr(tmp->arg, '=') && tmp->arg[0] != '=' && tmp->arg[ft_strlen(tmp->arg) - 1] != '=')
		{
			*env = add_env_var(tmp->arg, *env);
		}
		else
		{
			ft_printf("Required format VARNAME=value `%s'\n", tmp->arg);
			return;
		}
		tmp = tmp->next;
	}
}
