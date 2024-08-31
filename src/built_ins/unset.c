/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:53:58 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 13:30:09 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Helper function to remove an environment variable from the environment array
char **remove_env_var(t_gc *gc, char **env, int index)
{
    int i, j;
    int env_len = ft_env_len(env);
    char **new_env = ft_gc_malloc(gc, sizeof(char *) * env_len);

    if (!new_env)
    {
        ft_printf("Error: Memory allocation failed\n");
        return env;
    }

    for (i = 0, j = 0; i < env_len; i++)
    {
        if (i != index)
        {
            new_env[j++] = env[i];
        }
    }
    new_env[j] = NULL;

    return new_env;
}

// Function to remove an environment variable
void built_in_unset(t_shell *shell)
{
    t_gc *gc;
    char **args;
    char ***env;
    int var_index;

    if (!shell || !shell->cmds_head || !shell->cmds_head->args)
    {
        ft_printf("Error: Invalid shell structure\n");
        return;
    }

    gc = &shell->gc;
    env = &shell->env;
    args = shell->cmds_head->args;  // Get the arguments array

    if (!args[1])
    {
        ft_printf("unset: not enough arguments\n");
        return;
    }

    for (int i = 1; args[i]; i++)
    {
        var_index = find_var_in_env(*env, args[i]);
        if (var_index != -1)
        {
            // Remove the environment variable
            *env = remove_env_var(gc, *env, var_index);
        }
        else
        {
            ft_printf("unset: `%s': not a valid identifier\n", args[i]);
        }
    }
}