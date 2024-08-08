/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:08:32 by afoth             #+#    #+#             */
/*   Updated: 2024/08/08 17:01:02 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	output_redirection_ahead(t_arg *second_arg)
{
	while (second_arg != NULL)
	{
		if (second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND)
		{
			return (1);
		}
		second_arg = second_arg->next;
	}
	return (0);
}

int	multiple_pipes(t_gc *gc, t_arg *first_arg, t_arg *second_arg, int fd_input)
{

	int		fd[2];
	int		fd2[2];
	static bool	prev_arg_taken;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return(-1);
	}
	if (fd_input != -1 && fd_input != -2)
	{
		if (dup2(fd_input, fd[1]) == -1)
		{
			perror("dup2");
			close(fd[0]);
			close(fd[1]);
			return(-1);
		}
		close(fd_input);
	}
	if (prev_arg_taken == false)
		process_left_arg(gc, fd, first_arg);

	if (redirection_ahead(second_arg) == 1)
	{
		if (pipe(fd2) == -1)
		{
			perror("pipe");
			return(-1);
		}
		process_right_arg_with_pipes_ahead(gc, fd, fd2, second_arg);
		prev_arg_taken = true;
		return(fd2[0]);
	}
	else
	{
		if (output_redirection_ahead(second_arg) != 1)
			process_right_arg(gc, fd, second_arg);
		else
			return(fd[0]);
	}
	return (-2);
}


void	process_left_arg(t_gc *gc, int *fd, t_arg *first_arg)
{
	pid_t	pid1;

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid1 > 0)
	{
		//printf("\nchild1\n");
		//printf("pid1: %d\n", pid1);
	}
	if (pid1 == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd[0]);
			close(fd[1]);
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(gc, first_arg);
		exit(0);
	}
	waitpid(pid1, NULL, 0);
	close(fd[1]);
}

int	redirection_ahead(t_arg *second_arg)
{
	while (second_arg != NULL)
	{
		if (second_arg->type == PIPE || second_arg->type == REDIRECTION_IN || second_arg->type == HEREDOC)
		{
			return (1);
		}
		second_arg = second_arg->next;
	}
	return (0);
}

void process_right_arg(t_gc *gc, int *fd, t_arg *second_arg)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid2 > 0)
	{
		//printf("\nchild2\n");
		//printf("pid2: %d\n", pid2);
	}
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2");
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(gc, second_arg);
		exit(0);
	}
	//make sure to close the pipes in the parent process
	close(fd[0]);
	waitpid(pid2, NULL, 0);
}

void	process_right_arg_with_pipes_ahead(t_gc *gc, int *fd, int *fd2, t_arg *second_arg)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid2 > 0)
	{
		//printf("\nmorechild\n");
		//printf("pidmore: %d\n", pid2);
	}
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2");
			return ;
		}
		if (dup2(fd2[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd2[0]);
			close(fd2[1]);
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		//close(fd2[0]);
		close(fd2[1]);
		redirect_execve_args(gc, second_arg);
		exit(0);
	}
	waitpid(pid2, NULL, 0);
}
//what return type should be?
void	simple_pipe_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid1 == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd[0]);
			close(fd[1]);
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(gc, tmp);
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
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2");
			return ;
		}
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(gc, head->next);
		exit(0);
	}
	if (pid2 > 0)
	{
		//printf("\nchild2\n");
		//printf("pid2: %d\n", pid2);
	}
	//make sure to close the pipes in the parent process
	close(fd[0]);
	waitpid(pid2, NULL, 0);
}

