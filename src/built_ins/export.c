/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/02 19:47:24 by mokutucu         ###   ########.fr       */
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
void print_export_env(t_gc *gc, char **env)
{
    int env_len;
    char **sorted_env;
    int i;

    env_len = ft_env_len(env);
    sorted_env = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
    if (!sorted_env)
    {
        perror("Failed to allocate memory for sorted environment");
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (i < env_len)
    {
        sorted_env[i] = ft_shell_strdup(gc, env[i]);
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

void built_in_export(t_shell *shell)
{
    t_gc *gc;
    char **args;
    char **env;
    int i;

    // Extract the environment and arguments
    gc = &shell->gc;
    env = shell->env; // Directly use the environment
    args = shell->cmds_head->args;

    // If there are no additional arguments, print the environment
    if (!args)
    {
        print_export_env(gc, env);
        return;
    }

    // Process each argument
    i = 0; // Start from the first argument (skipping the command itself)
    while (args[i])
    {
        char *arg = args[i];
        // Validate and update environment variable
        if (arg && ft_strchr(arg, '=') && arg[0] != '=' && arg[ft_strlen(arg) - 1] != '=')
        {
            ft_printf("Adding or updating env var: %s\n", arg);
            env = change_or_add_env_var(gc, arg, env); // Update env directly
        }
        else
        {
            ft_printf("export: `%s': not a valid identifier\n", arg);
        }
        i++; // Move to the next argument
    }
    shell->env = env; // Ensure the shell's environment is 
}
