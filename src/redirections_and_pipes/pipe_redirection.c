/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:08:32 by afoth             #+#    #+#             */
/*   Updated: 2024/06/27 21:56:21 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//what return type should be?
void	pipe_redirection(t_arg *head, t_arg *tmp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("pipe");
		return ;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("fork");
		return ;
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2");
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(tmp);
		exit(0);
	}
	if (pid1 > 0)
	{
		printf("\nchild1\n");
		printf("pid1: %d\n", pid1);
	}
	waitpid(pid1, NULL, 0);
	close(fd[1]);
	pid2 = fork();
	if (pid2 == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("fork");
		return ;
	}
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2");
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(head->next);
		exit(0);
	}
	if (pid2 > 0)
	{
		printf("\nchild2\n");
		printf("pid2: %d\n", pid2);
	}
	//make sure to close the pipes in the parent process
	close(fd[0]);
	waitpid(pid2, NULL, 0);
}

