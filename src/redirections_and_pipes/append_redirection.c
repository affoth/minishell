/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:59:06 by afoth             #+#    #+#             */
/*   Updated: 2024/07/02 13:39:02 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int stdout_save;

	//check_file_writable(head->next->arg);
	fd = open(head->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		// exit(EXIT_FAILURE);
	}
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
	{
		perror("dup");
		// ft_gc_free();
		// exit(EXIT_FAILURE);
	}
	dup2_check = dup2(fd, STDOUT_FILENO);
	if (dup2_check == -1)
	{
		perror("dup2");
		// ft_gc_free();
		// exit(EXIT_FAILURE);
	}
	// printf("Output redirection: %s\n", head->next->arg);
	// printf("fd: %d\n", fd);
	// printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
	//Execute command
	redirect_execve_args(gc, tmp);
	//dup2(stdout_save, STDOUT_FILENO); //this is not needed ???
	if (dup2(stdout_save, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		// ft_gc_free();
		// exit(EXIT_FAILURE);
	}

	close(stdout_save);
	close(fd);
}
