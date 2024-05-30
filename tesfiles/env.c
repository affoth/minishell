/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:25:37 by afoth             #+#    #+#             */
/*   Updated: 2024/05/30 15:18:19 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//function to expand environment variables for example $PATH
// DEL needs testing and implementation of exit_status()
char	*ft_expand_env(char *env)
{
	char	*path;
	char	*temp;


	// if (env == "?")
	// 	exit_status(); //DEL needs to be implemented should return the exit status of the last command
	temp = getenv(env);
	if (temp != NULL)
	{
		path = ft_shell_strdup(temp);
		return (path);
	}
	else
	{
		printf("The %s environment variable is not set.\n", env);
	}
	return 0;
}
