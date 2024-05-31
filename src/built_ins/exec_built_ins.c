/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_ins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:37:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/31 19:36:52 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void exec_built_ins(t_arg *args_head)
{
    if (ft_strcmp(args_head->arg, "cd") == 0)
        built_in_cd(args_head);
}
