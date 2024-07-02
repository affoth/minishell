/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 20:07:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 13:36:16 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	ft_words(char const *s, char c)
{
	size_t	count;
	bool	quote;
	char	quote_char;

	quote_char = 0;
	count = 0;
	quote = false;
	while (*s)
	{
		while (*s == c && !quote)
			s++;
		skip_quoted_string(&s, &quote, &quote_char);
		if (*s && (quote || *s != c))
		{
			count++;
			while (*s && (quote || *s != c))
			{
				if (*s == quote_char)
					quote = !quote;
				s++;
			}
		}
	}
	return (count);
}

static void	*ft_allocate(t_gc *gc, const char *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_gc_malloc(gc, (end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_shell_split(t_gc *gc, const char *s, char c)
{
	char	**array;
	size_t	i;
	size_t	j;
	bool	quote;
	int		index;

	array = ft_gc_malloc(gc, (ft_words(s, c) + 1) * sizeof(char *));
	if (!s || !array)
		return (NULL);
	assign(&i, &j, &index, &quote);
	while (i <= ft_strlen(s))
	{
		handle_quote_split(s, i, &quote);
		if ((s[i] != c || quote == true) && index < 0)
			index = i;
		else if (((s[i] == c && quote == false) || \
		i == ft_strlen(s)) && index >= 0)
		{
			array[j++] = ft_allocate(gc, s, index, i);
			index = -1;
		}
		i++;
	}
	array[j] = NULL;
	return (array);
}
