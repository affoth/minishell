/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:53:40 by afoth             #+#    #+#             */
/*   Updated: 2024/09/11 20:26:26 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void handle_quote_split(const char *s, size_t i, bool *quote)
{
    static char quote_char = 0;

    if (s[i] == '\'' || s[i] == '\"')
    {
        if (*quote == false)
        {
            *quote = true;
            quote_char = s[i];
        }
        else if (*quote == true && s[i] == quote_char)
        {
            *quote = false;
            quote_char = 0;
        }
    }
}


void	skip_quoted_string(const char **s, bool *quote, char *quote_char)
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

int	ft_quotes_not_closed(const char *line)
{
	bool	in_single_quote = false;
	bool	in_double_quote = false;
	size_t	i = 0;

	while (line[i] != '\0')
	{
		if (line[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote; // Toggle single quote
		else if (line[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote; // Toggle double quote
		i++;
	}

	// If either single or double quote is still open, return an error
	if (in_single_quote || in_double_quote)
	{
		ft_printf("Error: Quotes not closed\n");
		return (1); // Return error code for unclosed quotes
	}

	return (0); // No unclosed quotes
}


