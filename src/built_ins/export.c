/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/20 21:11:16 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_export_env(char **env)
{
	while (*env)
	{
		ft_printf("declare -x %s\n", *env);
		env++;
	}
}

// Handle the export built-in command
void built_in_export(t_arg *args_head, char ***env)
{
	t_arg *tmp;

	tmp = args_head->next;
	if (!tmp)
	{
		print_export_env(*env);
		return;
	}

	while (tmp)
	{
		if (ft_strchr(tmp->arg, '=') && tmp->arg[0] != '=' && tmp->arg[ft_strlen(tmp->arg) - 1] != '=')
		{
			*env = change_or_add_env_var(tmp->arg, *env);
		}
		else
		{
			ft_printf("Required format VARNAME=value `%s'\n", tmp->arg);
			return;
		}
		tmp = tmp->next;
	}
}
