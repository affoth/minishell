/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/19 15:18:25 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_swap(char **a, char **b)
{
	char *temp = *a;
	*a = *b;
	*b = temp;
}
void	ft_sort(char **env, int len)
{
	int i;
	int j;
	int min_idx;

	i = 0;
	while (i < len - 1)
	{
		min_idx = i;
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(env[j], env[min_idx]) < 0)
			{
				min_idx = j;
			}
			j++;
		}
		ft_swap(&env[i], &env[min_idx]);
		i++;
	}

}

void print_export_env(t_gc *gc, char **env)
{
	int		env_len;
	char	**sorted_env;
	int		i;

	env_len = ft_env_len(env);
	sorted_env = (char **)ft_gc_malloc(gc, (sizeof(char *) * (env_len + 1)));
	i = 0;
	while (i < env_len) {
		sorted_env[i] = ft_strdup(env[i]);
		i++;
	}
	sorted_env[env_len] = NULL; // Null-terminate the array

	// Sort the sorted_env array alphabetically using selection sort
	ft_sort(sorted_env, env_len);
	i = 0;
	while (i < env_len)
	{
		ft_printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
}

// Handle the export built-in command
void built_in_export(t_gc *gc, t_arg *args_head, char ***env)
{
	t_arg *tmp;

	tmp = args_head->next;
	if (!tmp)
	{
		print_export_env(gc, *env);
		return;
	}

	while (tmp)
	{
		if (ft_strchr(tmp->arg, '=') && tmp->arg[0] != '=' && tmp->arg[ft_strlen(tmp->arg) - 1] != '=')
		{
			*env = change_or_add_env_var(gc, tmp->arg, *env);
		}
		else
		{
			ft_printf("Required format VARNAME=value `%s'\n", tmp->arg);
			return;
		}
		tmp = tmp->next;
	}
}
