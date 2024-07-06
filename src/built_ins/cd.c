/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 14:04:01 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to change directory
void built_in_cd(t_gc *gc, t_arg *args_head, char ***env)
{
	char *target_dir;
	char *oldpwd;
	char *newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		return;
	}

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
			free(oldpwd);
			return;
		}
	}

	// Attempt to change directory
	if (chdir(target_dir) == -1)
	{
		// Print error message along with the target directory and error details
		ft_printf("Error changing directory to %s\n", target_dir);
		free(oldpwd);
		return;
	}

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("getcwd");
		free(oldpwd);
		return;
	}

	// Update environment variables
	*env = change_or_add_env_var(gc, ft_shell_strjoin(gc, "OLDPWD=", oldpwd), *env);
	*env = change_or_add_env_var(gc, ft_shell_strjoin(gc, "PWD=", newpwd), *env);

	// Free allocated memory
	free(oldpwd);
	free(newpwd);
}

