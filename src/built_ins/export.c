/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 13:25:16 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Swap two strings in an array
void ft_swap(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

// Selection sort to sort environment variables alphabetically
void ft_sort(char **env, int len)
{
    int i;
    int j;
    int min_idx;

    i = 0;
    while (i < len - 1)
    {
        min_idx = i;
        j = i + 1;
        while (j < len)
        {
            if (ft_strcmp(env[j], env[min_idx]) < 0)
            {
                min_idx = j;
            }
            j++;
        }
        ft_swap(&env[i], &env[min_idx]);
        i++;
    }
}

// Function to print the sorted export environment
void print_export_env(t_gc *gc, char **env)
{
    int     env_len;
    char    **sorted_env;
    int     i;

    env_len = ft_env_len(env);
    sorted_env = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
    i = 0;
    while (i < env_len)
    {
        sorted_env[i] = ft_strdup(env[i]);
        i++;
    }
    sorted_env[env_len] = NULL; // Null-terminate the array

    // Sort the sorted_env array alphabetically
    ft_sort(sorted_env, env_len);

    i = 0;
    while (i < env_len)
    {
        ft_printf("declare -x %s\n", sorted_env[i]);
        i++;
    }

}

// Handle the export built-in command
void built_in_export(t_shell *shell)
{
    t_gc *gc;
    char **args;
    char ***env;

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
        print_export_env(gc, *env);
        return;
    }

    for (int i = 1; args[i]; i++)
    {
        char *arg = args[i];
        if (arg && ft_strchr(arg, '=') && arg[0] != '=' && arg[ft_strlen(arg) - 1] != '=')
        {
            *env = change_or_add_env_var(gc, arg, *env);
        }
        else
        {
            ft_printf("export: `%s': not a valid identifier\n", arg);
        }
    }
}
