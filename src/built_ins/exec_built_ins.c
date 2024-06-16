/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_ins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:37:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/16 13:45:20 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern char **environ;

void exec_built_ins(t_arg *args_head)
{
    if (ft_strcmp(args_head->arg, "cd") == 0)
        built_in_cd(args_head);
    if (ft_strcmp(args_head->arg, "echo") == 0)
        built_in_echo(args_head);
    if (ft_strcmp(args_head->arg, "pwd") == 0)
        built_in_pwd();
    if (ft_strcmp(args_head->arg, "env") == 0)
        built_in_env(environ);
    if (ft_strcmp(args_head->arg, "export") == 0)
        built_in_export(args_head, &environ);
}
