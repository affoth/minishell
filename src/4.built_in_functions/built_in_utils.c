/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:09:18 by afoth             #+#    #+#             */
/*   Updated: 2024/09/17 23:23:10 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
size_t	process_quotes(char *result, const char *str, size_t i, size_t *j)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		result[(*j)++] = str[i++];
	if (str[i] == quote)
		i++;
	return (i);
}
char	*strip_quotes(t_gc *gc, const char *str)
{
	size_t	i;
	size_t	j;
	char	*result;

	i = 0;
	j = 0;
	result = (char *)ft_gc_malloc(gc, ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = process_quotes(result, str, i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
