/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:46:30 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/28 00:03:01 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Remove the environment variable at the given index
char **remove_env_var(t_gc *gc, char **env, int env_idx)
{
    int env_len;
    int i;

    env_len = ft_env_len(env);
    char **new_env = ft_gc_malloc(gc, ((env_len) * sizeof(char *)));
    if (!new_env)
    {
        perror("No memory left for new_env");
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (i < env_idx)
    {
        new_env[i] = env[i];
        i++;
    }
    while (env[i])
    {
        new_env[i] = env[i + 1];
        i++;
    }
    new_env[env_len - 1] = NULL;
    return new_env;
}

// Handle the unset built-in command
void built_in_unset(t_gc *gc, t_arg *args_head, char ***env)
{
    t_arg *tmp;
    int env_idx;

    tmp = args_head->next;
    while (tmp)
    {
        if (strchr(tmp->arg, '=') || tmp->arg[0] == '=' || tmp->arg[strlen(tmp->arg) - 1] == '=')
        {
            ft_printf("UNSET: VARNAME needs to exist in env. NOT FOUND:`%s'\n", tmp->arg);
        }
        env_idx = find_var_in_env(*env, tmp->arg);
        if (env_idx != -1)
        {
            *env = remove_env_var(gc, *env, env_idx);
        }
        tmp = tmp->next;
    }
}