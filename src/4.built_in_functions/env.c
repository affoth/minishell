/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:43:12 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/11 15:31:12 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to print environment variables
int built_in_env(t_shell *shell)
{
    char **env = shell->env;

    if (!env) {
        // Return 1 if the environment is not set
        return 1;
    }

    while (*env)
    {
        if (write(STDOUT_FILENO, *env, ft_strlen(*env)) == -1) {
            // Return 1 if write fails
            return 1;
        }
        if (write(STDOUT_FILENO, "\n", 1) == -1) {
            // Return 1 if write fails
            return 1;
        }
        env++;
    }

    // Return 0 for success
    return 0;
}
