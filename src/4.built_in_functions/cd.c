/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:41:21 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/11 15:31:15 by mokutucu         ###   ########.fr       */
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
        ft_printf("Error: shell or command list is NULL.\n");
        return 1; // Error code
    }

    // Get the current working directory and save it as OLDPWD
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd) {
        perror("getcwd");
        return 1; // Error code
    }

    // Determine the target directory
    if (shell->cmds_head->args && shell->cmds_head->args[0])
    {
        target_dir = shell->cmds_head->args[0];
    }
    else if (shell->cmds_head->flags && shell->cmds_head->flags[0] && ft_strcmp(shell->cmds_head->flags[0], "-") == 0)
    {
        // Retrieve OLDPWD from the shell's environment
        int index = find_var_in_env(shell->env, "OLDPWD");
        if (index != -1) {
            target_dir = strchr(shell->env[index], '=') + 1;
        } else {
            ft_printf("cd: OLDPWD not set\n");
            free(oldpwd);
            return 1; // Error code
        }
    }
    else
    {
        // Retrieve HOME from the shell's environment
        int index = find_var_in_env(shell->env, "HOME");
        if (index != -1) {
            target_dir = strchr(shell->env[index], '=') + 1;
        } else {
            ft_printf("cd: HOME not set\n");
            free(oldpwd);
            return 1; // Error code
        }
    }

    // Change to the target directory
    if (chdir(target_dir) != 0) {
        perror("cd");
        free(oldpwd);
        return 1; // Error code
    }

    // Get the new working directory and update PWD and OLDPWD
    newpwd = getcwd(NULL, 0);
    if (!newpwd) {
        perror("getcwd");
        free(oldpwd);
        return 1; // Error code
    }

    // Update environment variables
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("OLDPWD=", oldpwd), shell->env);
    shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", newpwd), shell->env);

    free(oldpwd);
    free(newpwd);

    return 0; // Success code
}
