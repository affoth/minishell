/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_ins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:37:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/08 17:09:42 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// is built-in command
int is_built_in(char *arg)
{
	if (ft_strcmp(arg, "cd") == 0)
		return 1;
	if (ft_strcmp(arg, "echo") == 0)
		return 1;
	if (ft_strcmp(arg, "pwd") == 0)
		return 1;
	if (ft_strcmp(arg, "env") == 0)
		return 1;
	if (ft_strcmp(arg, "export") == 0)
		return 1;
	if (ft_strcmp(arg, "unset") == 0)
		return 1;
	if (ft_strcmp(arg, "exit") == 0)
		return 1;
	return 0;
}

// Function to execute built-in commands
void exec_built_ins(t_gc *gc, t_arg *args_head)
{
	//printf("exec_built_ins\n");
	if (ft_strcmp(args_head->arg, "cd") == 0)
		built_in_cd(gc, args_head, &environ);
	if (ft_strcmp(args_head->arg, "echo") == 0)
		built_in_echo(args_head);
	if (ft_strcmp(args_head->arg, "pwd") == 0)
		built_in_pwd();
	if (ft_strcmp(args_head->arg, "env") == 0)
		built_in_env(environ);
	if (ft_strcmp(args_head->arg, "export") == 0)
		built_in_export(gc, args_head, &environ);
	if (ft_strcmp(args_head->arg, "unset") == 0)
		built_in_unset(gc, args_head, &environ);
	if (ft_strcmp(args_head->arg, "exit") == 0)
		built_in_exit(args_head);
}
