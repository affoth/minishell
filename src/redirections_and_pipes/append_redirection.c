/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:59:06 by afoth             #+#    #+#             */
/*   Updated: 2024/08/22 15:40:26 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	simple_append_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int stdout_save;

	fd = open(head->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
	{
		perror("dup");
		return;
	}
	dup2_check = dup2(fd, STDOUT_FILENO);
	if (dup2_check == -1)
	{
		perror("dup2");
		close(stdout_save);
		close(fd);
		return;
	}

	// Execute the command
	redirect_execve_args(gc, tmp);

	if (dup2(stdout_save, STDOUT_FILENO) == -1)
		perror("dup2");

	close(stdout_save);
	close(fd);
}

