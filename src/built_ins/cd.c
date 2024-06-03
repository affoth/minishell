/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/03 19:11:51 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void built_in_cd(t_arg *args_head)
{
	char *target_dir;

	target_dir = NULL;
	// Determine the target directory based on the argument
	if (args_head->next)
	{
		target_dir = args_head->next->arg;
	}
	else
	{
		target_dir = getenv("HOME");
		if (!target_dir)
		{
			ft_printf("Error: HOME environment variable not set.\n");
			return;
		}
	}

	// Attempt to change directory
	if (chdir(target_dir) == -1)
	{
		// Print error message along with the target directory and error details
		ft_printf("Error changing directory to %s\n", target_dir);
	}
}
