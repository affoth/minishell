/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:49:06 by afoth             #+#    #+#             */
/*   Updated: 2024/09/19 22:11:54 by mokutucu         ###   ########.fr       */
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
