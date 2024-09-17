/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:35:01 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 23:19:22 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Function to remove quotes from a string.
It removes both single and double quotes while keeping the content inside intact.
 */
static char	*strip_quotes_static(t_gc *gc, const char *str)
{
	char	*result;
	size_t	i;
	size_t	j;
	bool	in_single_quote;
	bool	in_double_quote;

	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	result = ft_gc_malloc(gc, ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/*
 * Handles the redirection operators, tokenizes them, and manages quotes.
 */
// Detect and split redirection operators and pipes
// Add previous token before the special operator
// Handle double redirection (<< or >>)
// "<< " or ">> "
// Remove quotes
 // Handle single redirection (< or >)
//DEL ME deleted multiple char *token
static void	ft_handle_redirection(t_gc *gc, const char *s, char **array, size_t *index)
{
	size_t	i;
	bool	in_quote;
	char	quote_char;
	size_t	start;
	char	*token;

	i = 0;
	in_quote = false;
	quote_char = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (!in_quote)
			{
				in_quote = true;
				quote_char = s[i];
			}
			else if (s[i] == quote_char)
			{
				in_quote = false;
				quote_char = 0;
			}
		}
		if ((s[i] == '<' || s[i] == '>' || s[i] == '|') && !in_quote)
		{
			if (i > start)
			{
				token = ft_shell_strndup(gc, s + start, i - start);
				array[*index] = strip_quotes_static(gc, token);
				(*index)++;
			}
			if (s[i] == '<' || s[i] == '>')
			{
				if (s[i + 1] == s[i])
				{
					array[*index] = ft_gc_malloc(gc, 3);
					strncpy(array[*index], s + i, 2);
					array[*index][2] = '\0';
					(*index)++;
					i++;
				}
				else
				{
					array[*index] = ft_gc_malloc(gc, 2);
					strncpy(array[*index], s + i, 1);
					array[*index][1] = '\0';
					(*index)++;
				}
			}
			else if (s[i] == '|')
			{
				array[*index] = ft_gc_malloc(gc, 2);
				strncpy(array[*index], s + i, 1);
				array[*index][1] = '\0';
				(*index)++;
			}
			start = i + 1;
		}
		else if (s[i] == ' ' && !in_quote)
		{
			if (i > start)
			{
				token = ft_shell_strndup(gc, s + start, i - start);
				array[*index] = strip_quotes_static(gc, token);
				(*index)++;
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start)
	{
		token = ft_shell_strndup(gc, s + start, i - start);
		array[*index] = strip_quotes_static(gc, token);
		(*index)++;
	}
}

/*
 * Splits the input string into tokens while handling redirections and quotes.
  // Add some buffer for additional tokens (like redirection)
 */
char	**ft_split_redirections(t_gc *gc, const char *s)
{
	char	**array;
	size_t	num_tokens;
	size_t	index;

	index = 0;
	if (!s)
		return (NULL);
	num_tokens = ft_words(s, ' ') + 10;
	array = ft_gc_malloc(gc, num_tokens * sizeof(char *));
	if (!array)
		return (NULL);
	ft_handle_redirection(gc, s, array, &index);
	array[index] = NULL;
	return (array);
}
