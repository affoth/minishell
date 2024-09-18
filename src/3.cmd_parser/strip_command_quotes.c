/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_command_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:00:26 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/18 18:55:54 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	toggle_quotes(char c, bool *in_single_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
	{
		*in_single_quote = !(*in_single_quote);
		return (false);
	}
	if (c == '\"' && !(*in_single_quote))
	{
		*in_double_quote = !(*in_double_quote);
		return (false);
	}
	return (true);
}

static bool	append_char(t_gc *gc, char **result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_shell_strjoin(gc, *result, temp);
	if (!new_result)
	{
		free(*result);
		*result = NULL;
		return (false);
	}
	free(*result);
	*result = new_result;
	return (true);
}

char	*strip_redundant_quotes(t_gc *gc, const char *str)
{
	char	*result;
	bool	in_single_quote;
	bool	in_double_quote;
	int		i;

	result = ft_shell_strdup(gc, "");
	if (!result)
		return (NULL);
	in_single_quote = false;
	in_double_quote = false;
	i = 0;
	while (str[i])
	{
		if (!toggle_quotes(str[i], &in_single_quote, &in_double_quote))
		{
			i++;
			continue ;
		}
		if (!append_char(gc, &result, str[i]))
		{
			return (NULL);
		}
		i++;
	}
	return (result);
}
