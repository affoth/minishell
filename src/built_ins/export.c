/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 20:00:31 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/03 20:10:42 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// // Function to set environment variables
// void ft_setenv(char *key, char *value, char ***env)
// {
// 	int i;
// 	int j;
// 	char **new_env;

// 	i = 0;
// 	while ((*env)[i])
// 		i++;
// 	new_env = (char **)ft_gc_malloc(sizeof(char *) * (i + 2));
// 	if (!new_env)
// 		return;
// 	i = 0;
// 	while ((*env)[i])
// 	{
// 		new_env[i] = ft_shell_strdup((*env)[i]);
// 		i++;
// 	}
// 	new_env[i] = ft_shell_strjoin(key, "=");
// 	if (value)
// 		new_env[i] = ft_shell_strjoin(new_env[i], value);
// 	free((*env));
// 	new_env[i + 1] = NULL;
// 	(*env) = new_env;
// }
// char **ft_arg_to_array(t_arg *args)
// {
// 	int i;
// 	t_arg *temp;
// 	char **args_array;

// 	i = 0;
// 	temp = args;
// 	while (temp)
// 	{
// 		i++;
// 		temp = temp->next;
// 	}
// 	args_array = (char **)ft_gc_malloc(sizeof(char *) * (i + 1));
// 	if (!args_array)
// 		return (NULL);
// 	i = 0;
// 	temp = args;
// 	while (temp)
// 	{
// 		args_array[i] = ft_shell_strdup(temp->arg);
// 		i++;
// 		temp = temp->next;
// 	}
// 	args_array[i] = NULL;
// 	return (args_array);
// }

// void built_in_export(t_arg *args_head, char ***env)
// {
// 	t_arg *args;
// 	char **args_array;

// 	args = args_head->next;
// 	args_array = ft_arg_to_array(args);
// 	if (args_array)
// 	{
// 		ft_setenv(args_array[0], args_array[1], env);
// 		free(args_array);
// 	}
// }
