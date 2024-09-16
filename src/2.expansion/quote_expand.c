/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:55:59 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 23:01:17 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to expand environment variable from custom shell environment
char *ft_expand_env(t_shell *shell, char *env)
{
    int index;
    char *value = NULL;

    // Find the environment variable in the custom environment
    index = find_var_in_env(shell->env, env);
    if (index != -1)
    {
        value = strchr(shell->env[index], '=') + 1;
        return ft_shell_strdup(&shell->gc, value); // Return the value found in shell's environment
    }
    else
    {
        return ft_shell_strdup(&shell->gc, ""); // Return empty string if the variable is not set
    }
}

int check_if_in_single_quote(char *input, size_t i)
{
    size_t j = 0;

    while (j < i)
    {
        if (input[j] == '\'')
            return (1);
        j++;
    }
    return (0);
}

// Function to calculate the length of the expanded string
size_t calculate_expanded_length(t_shell *shell, char *input, int exit_status)
{
    size_t len = 0;
    size_t i = 0;
    char *env;
    char *expanded;
    char *exit_status_str;
    size_t exit_status_len;

    // Convert exit_status to string and get its length
    exit_status_str = ft_itoa(exit_status);
    if (!exit_status_str)
        return 0; // Handle memory allocation failure
    exit_status_len = ft_strlen(exit_status_str);

    while (input[i])
    {
        if (input[i] == '$')
        {
            size_t start = i + 1;
            if (input[start] == '?')
            {
                // Handle $?
                len += exit_status_len;
                i += 2; // Skip "$?"
            }
            else
            {
                while (input[start] && (isalnum(input[start]) || input[start] == '_'))
                    start++;
                env = ft_shell_substr(&shell->gc, input, i + 1, start - i - 1);
                expanded = ft_expand_env(shell, env);
                len += ft_strlen(expanded);
                free(env);
                free(expanded);
                i = start;
            }
        }
        else
        {
            len++;
            i++;
        }
    }
    free(exit_status_str); // Free the allocated memory for the exit status string
    return len;
}

char *expand_string(t_shell *shell, char *input, int exit_status)
{
    char *result;
    char *env;
    char *expanded;
    size_t i;
    size_t j;
    size_t len;

    if (!input)
        return NULL;

    // Calculate the required length for the result string
    len = calculate_expanded_length(shell, input, exit_status);
    result = (char *)ft_gc_malloc(&shell->gc, (sizeof(char) * (len + 1)));
    if (!result)
        return NULL;

    i = 0;
    j = 0;
    while (input[i])
    {
        if (input[i] == '$' && check_if_in_single_quote(input, i) == 0)
        {
            size_t start = i + 1;

            // Check for $? or environment variable
            if (input[start] == '?')
            {
                // Handle $?
                expanded = ft_itoa(exit_status);
                if (expanded)
                {
                    ft_strlcpy(result + j, expanded, len - j + 1);
                    j += ft_strlen(expanded);
                    i += 2; // Skip "$?"
                    free(expanded);
                }
            }
            else
            {
                // Check if the next character is a valid variable name or end of string
                while (input[start] && (isalnum(input[start]) || input[start] == '_'))
                    start++;

                if (start == i + 1)
                {
                    // No variable name after $, so treat it as a literal $
                    result[j++] = '$';
                    i++;
                }
                else
                {
                    // Handle environment variable
                    env = ft_shell_substr(&shell->gc, input, (i + 1), (start - i - 1));
                    expanded = ft_expand_env(shell, env); // Fetch from custom environment
                    ft_strlcpy(result + j, expanded, len - j + 1);
                    j += ft_strlen(expanded);
                    i = start;
                    free(env);
                    free(expanded);
                }
            }
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
}
