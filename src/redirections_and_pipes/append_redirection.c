/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:59:06 by afoth             #+#    #+#             */
/*   Updated: 2024/06/27 16:16:52 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_redirection(t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int stdout_save;

	//check_file_writable(head->next->arg);
	fd = open(head->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		ft_gc_free();
		exit(EXIT_FAILURE);
	}
	stdout_save = dup(STDOUT_FILENO);
	dup2_check = dup2(fd, STDOUT_FILENO);
	if (dup2_check == -1)
	{
		perror("dup2");
		ft_gc_free();
		exit(EXIT_FAILURE);
	}
	// printf("Output redirection: %s\n", head->next->arg);
	// printf("fd: %d\n", fd);
	// printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
	//Execute command
	redirect_execve_args(tmp);
	dup2(stdout_save, STDOUT_FILENO);
	close(stdout_save);
	close(fd);
}
