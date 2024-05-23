/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:25:37 by afoth             #+#    #+#             */
/*   Updated: 2024/05/23 15:19:01 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//function to expand environment variables for example $PATH
// DEL needs testing and implementation of exit_status()
/* char	*ft_expand_env(char *env)
{
	char *path;

	if (env == "?")
		exit_status(); //DEL needs to be implemented should return the exit status of the last command
	path = getenv(env);
	if (path != NULL)
	{
		return (path);
	}
	else
	{
		printf("The %s environment variable is not set.\n", env);
	}
	return 0;
} */
