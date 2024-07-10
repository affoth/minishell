/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:59:06 by afoth             #+#    #+#             */
/*   Updated: 2024/07/10 18:13:05 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



void	append_redirection(t_gc *gc, t_arg *first_arg, t_arg *second_arg, int fd_input)
{
	int	fd;
	int	stdout_save;
	pid_t	pid;
	printf("second_arg->arg: %s\n", second_arg->arg);
	//check_file_writable(head->next->arg);
	fd = open(second_arg->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror("open");
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
		perror("dup");


	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(fd);
		return;
	}
	if (pid > 0)
	{
		printf("\noutchild\n");
		printf("pidout: %d\n", pid);
	}
	if (pid == 0)
	{
		if (fd_input != -1 && fd_input != -2)
		{
			if (dup2(fd_input, STDIN_FILENO) == -1)
			{
				perror("dup2");
			}
			close(fd_input);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
		redirect_execve_args(gc, first_arg);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, NULL, 0);
	if (dup2(stdout_save, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdout_save);
	close(fd);
}


void	simple_append_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
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
