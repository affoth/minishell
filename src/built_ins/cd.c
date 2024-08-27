/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 15:28:17 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to change directory
void built_in_cd(t_shell *shell)
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
    if (shell->cmds_head->args_head->next)
    {
        target_dir = shell->cmds_head->args_head->next->arg;
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
        ft_printf("Error changing directory to %s: %s\n", target_dir, strerror(errno));
        free(oldpwd);
        return;
    }

    // Update the PWD environment variable
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        perror("getcwd");
        free(oldpwd);
        return;
    }

    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);

    free(oldpwd);
    free(newpwd);
}

