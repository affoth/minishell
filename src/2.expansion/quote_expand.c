/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:55:59 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 13:33:12 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to expand environment variable
char *ft_expand_env(t_gc *gc, char *env)
{
	char *temp;

	temp = getenv(env);
	if (temp != NULL)
	{
		return ft_shell_strdup(gc, temp);
	}
	else
	{
		return ft_shell_strdup(gc, ""); // Return an empty string if the variable is not set
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
size_t calculate_expanded_length(t_gc *gc, char *input)
{
	size_t len = 0;
	size_t i = 0;
	char *env;
	char *expanded;

	while (input[i])
	{
		if (input[i] == '$')
		{
			size_t start = i + 1;
			while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
				start++;
			env = ft_shell_substr(gc,input, i + 1, start - i - 1);
			expanded = ft_expand_env(gc, env);
			len += ft_strlen(expanded);
			i = start;
		}
		else
		{
			len++;
			i++;
		}
	}
	return len;
}

// Function to expand all environment variables in the input string
char *expand_string(t_gc *gc, char *input)
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
	len = calculate_expanded_length(gc, input);
	result = (char *)ft_gc_malloc(gc, (sizeof(char) * (len + 1)));
	if (!result)
		return NULL;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$' && check_if_in_single_quote(input, i) == 0)
		{
			size_t start = i + 1;
			while (input[start] && (isalnum(input[start]) || input[start] == '_'))
				start++;
			env = ft_shell_substr(gc, input, (i + 1), (start - i - 1));
			expanded = ft_expand_env(gc, env);
			ft_strlcpy(result + j, expanded, len - (j-1)); // Use ft_strlcpy instead of strcpy
			j += ft_strlen(expanded);
			i = start;
		}
		else
		{
			result[j++] = input[i++];
		}
	}
	result[j] = '\0';
	return result;
}