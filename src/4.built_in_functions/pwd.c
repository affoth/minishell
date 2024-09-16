/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:05:32 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 22:46:07 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Return codes
#define PWD_SUCCESS 0
#define PWD_ERROR 1

int built_in_pwd(t_shell *shell)
{
    char *current_dir;

    // Ensure shell struct is not NULL
    if (!shell || !shell->env) {
        ft_putstr_fd("pwd: internal error\n", STDERR_FILENO);
        return PWD_ERROR; // Error code
    }

    // Get the current working directory
    current_dir = getcwd(NULL, 0);
    if (!current_dir) {
        perror("getcwd");
        return PWD_ERROR; // Return error code if getcwd fails
    }

    // Print the current working directory to STDOUT
    write(STDOUT_FILENO, current_dir, ft_strlen(current_dir));
    write(STDOUT_FILENO, "\n", 1);

    // Update PWD in the custom environment if needed
    int index = find_var_in_env(shell->env, "PWD");
    if (index != -1) {
        shell->env = change_or_add_env_var(&shell->gc, ft_strjoin("PWD=", current_dir), shell->env);
    }

    free(current_dir);
    return PWD_SUCCESS; // Return success code
}

