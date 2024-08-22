/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 20:53:40 by afoth             #+#    #+#             */
/*   Updated: 2024/08/22 16:20:59 by mokutucu         ###   ########.fr       */
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

int ft_quotes_not_closed(char *line)
{
	int i;
	int single_quotes;
	int double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;

	while (line[i] != '\0')
	{
		if (line[i] == '\'')
			single_quotes++;
		if (line[i] == '"')
			double_quotes++;
		i++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		ft_printf("Quotations not closed\n");
		return (1);
	}
	return (0);
}
