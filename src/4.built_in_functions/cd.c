/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:51:53 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 23:15:03 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int built_in_cd(t_shell *shell)
{
    char *target_dir = NULL;
    char *oldpwd = NULL;
    char *newpwd = NULL;

    // Ensure shell and command list are not NULL
    if (!shell || !shell->cmds_head) {
        ft_putstr_fd("cd: internal error\n", STDERR_FILENO);
        return 1; // Error code
    }

    // Get the current working directory and save it as OLDPWD
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
        perror("getcwd");
        return 1; // Error code
    }

    // Determine the target directory
    if (shell->cmds_head->args && shell->cmds_head->args[0]) {
        target_dir = shell->cmds_head->args[0];
    } else {
        // Retrieve HOME from the shell's environment
        int index = find_var_in_env(shell->env, "HOME");
        if (index != -1) {
            target_dir = strchr(shell->env[index], '=') + 1;
        } else {
            ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
            free(oldpwd);
            return 1; // Error code
        }
    }

    // Check if the target directory is valid
    if (access(target_dir, F_OK) != 0) {
        ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
        free(oldpwd);
        return 1; // Error code
    }

    // Change to the target directory
    if (chdir(target_dir) != 0) {
        perror("cd");
        free(oldpwd);
        return 1; // Error code
    }

    // Get the new working directory
    newpwd = getcwd(NULL, 0);
    if (!newpwd) {
        perror("getcwd");
        free(oldpwd);
        return 1; // Error code
    }

    // Update OLDPWD in the environment
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);

    // Update PWD in the environment
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);

    free(oldpwd);
    free(newpwd);

    return 0; // Success code
}
