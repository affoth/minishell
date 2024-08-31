/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:54:56 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 13:18:02 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to change directory
void built_in_cd(t_shell *shell)
{
    char *target_dir;
    char *oldpwd;
    char *newpwd;

    if (!shell || !shell->cmds_head) {
        ft_printf("Error: shell or command list is NULL.\n");
        return;
    }

    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
        perror("getcwd");
        return;
    }

    // Determine the target directory based on the argument
    if (shell->cmds_head->args)
    {
        char **args = shell->cmds_head->args;

        if (args[1])
        {
            target_dir = args[1];
        }
        else
        {
            target_dir = getenv("HOME");
            if (!target_dir)
            {
                ft_printf("Error: HOME environment variable not set or empty.\n");
                free(oldpwd);
                return;
            }
        }
    }
    else
    {
        target_dir = getenv("HOME");
        if (!target_dir)
        {
            ft_printf("Error: HOME environment variable not set or empty.\n");
            free(oldpwd);
            return;
        }
    }

    if (chdir(target_dir) != 0)
    {
        perror("chdir");
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

    // Update the PWD and OLDPWD environment variables
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);

    free(oldpwd);
    free(newpwd);
}
