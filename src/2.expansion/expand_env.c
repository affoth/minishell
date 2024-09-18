/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:55:59 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/18 22:42:09 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to expand environment variable from custom shell environment
// Find the environment variable in the custom environment

char	*ft_expand_env(t_shell *shell, char *env)
{
	int		index;
	char	*value;

	value = NULL;
	index = find_var_in_env(shell->env, env);
	if (index != -1)
	{
		value = strchr(shell->env[index], '=') + 1;
		return (ft_shell_strdup(&shell->gc, value));
	}
	else
	{
		return (ft_shell_strdup(&shell->gc, ""));
	}
}

int	check_if_in_single_quote(char *input, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		if (input[j] == '\'')
			return (1);
		j++;
	}
	return (0);
}
