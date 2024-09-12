/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:45:00 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 14:30:53 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Helper function to remove an environment variable from the environment array
char **remove_env_var(t_gc *gc, char **env, int index)
{
    int i = 0;
    int j = 0;
    int env_len = ft_env_len(env);
    char **new_env = ft_gc_malloc(gc, sizeof(char *) * env_len);

    if (!new_env)
    {
        write(STDERR_FILENO, "unset: memory allocation failed\n", 32);
        return env;
    }

    while (i < env_len)
    {
        if (i != index)
        {
            new_env[j] = env[i];
            j++;
        }
        i++;
    }
    new_env[j] = NULL;

    return new_env;
}

int built_in_unset(t_shell *shell)
{
    t_gc *gc;
    char **args;
    char **env;
    int var_index;
    int i;

    gc = &shell->gc;
    env = shell->env;
    args = shell->cmds_head->args;  // Get the arguments array

    if (!args || !args[0])  // Check if there are arguments to unset
    {
        return 0;  // Return success code
    }

    i = 0;
    while (args[i])
    {
        var_index = find_var_in_env(env, args[i]);
        if (var_index != -1)
        {
            // Remove the environment variable
            env = remove_env_var(gc, env, var_index);
        }
        else
        {
            // Print an error message if variable not found
            write(STDERR_FILENO, "unset: ", 7);
            write(STDERR_FILENO, args[i], ft_strlen(args[i]));
            write(STDERR_FILENO, ": not found\n", 12);
        }
        i++;
    }

    shell->env = env; // Update the shell's environment pointer
    return 0; // Always return success
}
