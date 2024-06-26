/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:08:32 by afoth             #+#    #+#             */
/*   Updated: 2024/06/26 14:45:13 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//what return type should be?
void	pipe_redirection(t_arg *head, t_arg *tmp)
{
	if (count_pipes(head) > 1)
	{
		//mutiple_pipes(head, tmp);
	}
	else
	{
		single_pipe(head, tmp);
	}

}
void single_pipe(t_arg *head, t_arg *tmp)
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
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(tmp);
		exit(0);
	}
	if (pid1 > 0)
		printf("\nchild1\n");
	waitpid(pid1, NULL, 0);
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
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(head->next);
	}
	if (pid2 > 0)
		printf("\nchild2\n");
	//make sure to close the pipes in the parent process
	close(fd[0]);
	close(fd[1]);
	waitpid(pid2, NULL, 0);
}

/* void single_pipe(t_arg *head, t_arg *tmp)
{
    int     fd[2];
    pid_t   pid1;
    pid_t   pid2;

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
        printf("\nchild1\n");
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        redirect_execve_args(tmp);
        perror("execve"); // In case execve fails
        exit(1);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0); // Clean up the first child
        return ;
    }

    if (pid2 == 0)
    {
        printf("\nchild2\n");
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        redirect_execve_args(head);
        perror("execve"); // In case execve fails
        exit(1);
    }

    // Parent process should close pipe fds
    close(fd[0]);
    close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
} */
