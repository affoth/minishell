/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/21 15:28:27 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Update environment variable PWD


// Function to change directory
void built_in_cd(t_arg *args_head, char ***env)
{
	char *target_dir;
	char *oldpwd;;

	oldpwd = getcwd(NULL, 0);
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

	// update oldpwd
	*env = change_or_add_env_var(ft_strjoin("OLDPWD=", oldpwd), *env);
	*env = change_or_add_env_var(ft_strjoin("PWD=", getcwd(NULL, 0)), *env);
}
