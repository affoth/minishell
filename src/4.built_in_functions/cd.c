/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/02 17:44:19 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void built_in_cd(t_shell *shell)
{
    char *target_dir = NULL;
    char *oldpwd = NULL;
    char *newpwd = NULL;

    // Ensure shell and command list are not NULL
    if (!shell || !shell->cmds_head) {
        ft_printf("Error: shell or command list is NULL.\n");
        return;
    }

    // Get the current working directory and save it as OLDPWD
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
        perror("getcwd");
        return;
    }

    // Determine the target directory
    if (shell->cmds_head->args && shell->cmds_head->args[0]) {
        target_dir = shell->cmds_head->args[0];
    } else {
        target_dir = getenv("HOME");
        if (!target_dir) {
            ft_printf("cd: HOME not set\n");
            free(oldpwd);
            return;
        }
    }

    // Change to the target directory
    if (chdir(target_dir) != 0) {
        perror("cd");
        free(oldpwd);
        return;
    }

    // Get the new working directory and update PWD and OLDPWD
    newpwd = getcwd(NULL, 0);
    if (!newpwd) {
        perror("getcwd");
        free(oldpwd);
        return;
    }

    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);

    free(oldpwd);
    free(newpwd);
}
