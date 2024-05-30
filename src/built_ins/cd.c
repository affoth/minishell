/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/30 16:49:56 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void built_in_cd(t_arg *args_head) 
{
	if (args_head->next)
	{
		if (chdir(args_head->next->arg) == -1)
			perror("cd");
	}
	else
	{
		if (chdir(getenv("HOME")) == -1)
			perror("cd");
	}
}