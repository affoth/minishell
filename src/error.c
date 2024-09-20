/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:49:06 by afoth             #+#    #+#             */
/*   Updated: 2024/09/20 16:31:34 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	perror_fork(void)
{
	perror("fork");
	exit(EXIT_FORK_FAILED);
}

void	perror_malloc(void)
{
	perror("malloc");
	exit(EXIT_FAILURE);
}

void	perror_strdup(void)
{
	perror("strdup");
	exit(EXIT_FAILURE);
}

int	perror_pipe(void)
{
	perror("pipe");
	return (-1);
}

// Helper function to print error messages to STDERR
void	write_error(const char *msg, const char *arg)
{
	write(2, msg, strlen(msg));
	if (arg)
	{
		write(2, arg, strlen(arg));
	}
	write(2, "\n", 1);
}
