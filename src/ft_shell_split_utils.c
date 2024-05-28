/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_split_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:53:40 by afoth             #+#    #+#             */
/*   Updated: 2024/05/28 20:55:20 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_quote_split(const char *s, size_t i, bool *quote)
{
	char	quote_char;

	if (s[i] == '\'' || s[i] == '\"')
	{
		if (*quote == false)
		{
			*quote = true;
			quote_char = s[i];
		}
		else if (*quote == true && s[i] == quote_char)
			*quote = false;
	}
}

void	handle_quote_wordcount(const char **s, bool *quote, char *quote_char)
{
	if (**s == '\'' || **s == '\"')
	{
		*quote = !(*quote);
		*quote_char = **s;
		(*s)++;
	}
}

void	assign(size_t *i, size_t *j, int *index, bool *quote)
{
	*i = 0;
	*j = 0;
	*index = -1;
	*quote = false;
}
