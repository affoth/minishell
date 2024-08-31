/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:43:12 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 12:33:28 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to print environment variables
void built_in_env(t_shell *shell)
{
	char **env = shell->env;

	while (*env)
	{
		ft_printf("%s\n", *env);
		env++;
	}
}