/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 20:11:18 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void built_in_cd(t_shell *shell)
{
    char *target_dir = NULL;
    char *oldpwd = NULL;
    char *newpwd = NULL;
    int ret = 0;

    // Ensure shell and command list are not NULL
    if (!shell || !shell->cmds_head) {
        ft_printf("Error: shell or command list is NULL.\n");
        return;
    }

    // Get the current working directory
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
        perror("getcwd");
        return;
    }
    printf("oldpwd: %s\n", oldpwd);

    // Determine the target directory
    if (shell->cmds_head->args && shell->cmds_head->args[1]) {
        target_dir = shell->cmds_head->args[1];
        printf("Target directory from args: %s\n", target_dir);
    } else {
        target_dir = getenv("HOME");
        if (!target_dir) {
            ft_printf("Error: HOME environment variable not set or empty.\n");
            ret = 1;
        } else {
            printf("Target directory from HOME: %s\n", target_dir);
        }
    }

    // Change to the target directory if valid
    if (ret == 0) {
        if (chdir(target_dir) != 0) {
            perror("chdir");
            ret = 1;
        } else {
            // Get the new working directory
            newpwd = getcwd(NULL, 0);
            if (!newpwd) {
                perror("getcwd");
                ret = 1;
            } else {
                printf("newpwd: %s\n", newpwd);
                // Update environment variables
                shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);
                shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);
                free(newpwd);
            }
        }
    }

    free(oldpwd);
}
