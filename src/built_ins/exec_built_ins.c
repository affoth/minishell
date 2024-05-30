/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_ins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:37:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/30 16:27:57 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void exec_built_ins(t_arg *args_head)
{
    if (ft_strcmp(args_head->arg, "cd") == 0)
        built_in_cd(args_head);
    // else if (ft_strcmp(args_head->arg, "echo") == 0)
    //     built_in_echo(args_head);
    // else if (ft_strcmp(args_head->arg, "pwd") == 0)
    //     built_in_pwd();
    // else if (ft_strcmp(args_head->arg, "export") == 0)
    //     built_in_export(args_head);
    // else if (ft_strcmp(args_head->arg, "unset") == 0)
    //     built_in_unset(args_head);
    // else if (ft_strcmp(args_head->arg, "env") == 0)
    //     built_in_env();
    // else if (ft_strcmp(args_head->arg, "exit") == 0)
    //     built_in_exit(args_head);
    // else
    //     exec_command(args_head);
}