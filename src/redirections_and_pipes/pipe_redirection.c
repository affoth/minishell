/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:08:32 by afoth             #+#    #+#             */
/*   Updated: 2024/08/20 18:04:32 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	output_redirection_ahead(t_arg *second_arg)
{
    while (second_arg != NULL)
    {
        if (second_arg->type == REDIRECTION_OUT || second_arg->type == REDIRECTION_APPEND)
        {
            printf("Output redirection ahead detected.\n");
            return (1);
        }
        second_arg = second_arg->next;
    }
    printf("No output redirection ahead.\n");
    return (0);
}

int	redirection_ahead(t_arg *second_arg)
{
    while (second_arg != NULL)
    {
        if (second_arg->type == PIPE || second_arg->type == REDIRECTION_IN || second_arg->type == HEREDOC)
        {
            printf("Redirection ahead detected.\n");
            return (1);
        }
        second_arg = second_arg->next;
    }
    printf("No redirection ahead.\n");
    return (0);
}

int	multiple_pipes(t_gc *gc, t_arg *first_arg, t_arg *second_arg, int fd_input)
{
    int		fd[2];
    int		fd2[2];
    static bool	prev_arg_taken;

    printf("\nSTART multiple_pipes\n");
    printf("fd_input: %d\n", fd_input);
    printf("prev_arg_taken: %d\n", prev_arg_taken);

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return(-1);
    }
    printf("Pipe created: fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);

    if (fd_input != -1 && fd_input != -2)
    {
        if (dup2(fd_input, fd[0]) == -1)
        {
            perror("dup2");
            close(fd[0]);
            close(fd[1]);
            return(-1);
        }
		printf("fd[0]: %d, fd_input: %d\n", fd[0], fd_input);
        //close(fd_input);
        printf("Dup2 successful: fd_input -> fd[0]\n");
    }

    if (prev_arg_taken == false)
    {
        printf("Processing left argument.\n");
        process_left_arg(gc, fd, first_arg);
    }

    if (redirection_ahead(second_arg) == 1)
    {
        if (pipe(fd2) == -1)
        {
            perror("pipe");
            close(fd[0]);
            close(fd[1]);
            return(-1);
        }
        printf("Second pipe created: fd2[0]: %d, fd2[1]: %d\n", fd2[0], fd2[1]);
        process_right_arg_with_pipes_ahead(gc, fd, fd2, second_arg);
        prev_arg_taken = true;
        close(fd[0]);
        close(fd[1]);
		printf("RETURN fd2[1]: %d\n", fd2[1]);
        return(fd2[1]);
    }
    else
    {
        if (output_redirection_ahead(second_arg) != 1)
        {
            printf("Processing right argument.\n");
            process_right_arg(gc, fd, second_arg , fd_input, prev_arg_taken);
        }
        else
        {
            printf("Output redirection ahead, returning fd[0].\n");
            close(fd[1]);
            return(fd[0]);
        }
    }
    close(fd[0]);
    close(fd[1]);
	prev_arg_taken = false;
    return (-2);
}

void	process_left_arg(t_gc *gc, int *fd, t_arg *first_arg)
{
    pid_t	pid1;

    printf("Forking for left argument.\n");
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
        printf("Parent process: child1 pid: %d\n", pid1);
    }
    if (pid1 == 0)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd[0]);
            close(fd[1]);
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);//?
        printf("Executing left argument command.\n");
        redirect_execve_args(gc, first_arg);
        exit(EXIT_FAILURE);
    }
    waitpid(pid1, NULL, 0);
    close(fd[1]);
    printf("Left argument processing complete.\n");
}

void process_right_arg(t_gc *gc, int *fd, t_arg *second_arg, int fd_input, bool prev_arg_taken)
{
    pid_t	pid2;

    printf("Forking for right argument.");
	printf("fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);
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
        printf("Parent process: child2 pid: %d\n", pid2);
    }
    if (pid2 == 0)
    {
		if (prev_arg_taken) {
			printf("fd_input: %d\n", fd_input);
            if (dup2(fd_input, STDIN_FILENO) == -1) {
                close(fd_input);
                perror("dup2");
                exit(EXIT_FAILURE);
            }
			printf("fd[0] input : %d\n", fd[0]);
			printf("fd_input: %d\n", fd_input);
            close(fd_input);
        } else {
            if (dup2(fd[0], STDIN_FILENO) == -1) {
                close(fd[0]);
                close(fd[1]);
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd[0]);
        }
		close(fd[1]);
        printf("Executing right argument command.\n");
        redirect_execve_args(gc, second_arg);
        exit(EXIT_FAILURE);
    }
    close(fd[0]);
	close(fd[1]);
    waitpid(pid2, NULL, 0);
    printf("Right argument processing complete.\n");
}

void	process_right_arg_with_pipes_ahead(t_gc *gc, int *fd, int *fd2, t_arg *second_arg)
{
    pid_t	pid2;

    printf("Forking for right argument with pipes ahead.\n");
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        return ;
    }
    if (pid2 > 0)
    {
        printf("Parent process: morechild pid: %d\n", pid2);
    }
    if (pid2 == 0)
    {
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            close(fd[0]);
            close(fd[1]);
            close(fd2[0]);
            close(fd2[1]);
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd2[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd[0]);
            close(fd[1]);
            close(fd2[0]);
            close(fd2[1]);
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        printf("Executing right argument command with pipes ahead.\n");
        redirect_execve_args(gc, second_arg);
        exit(EXIT_FAILURE);
    }
    close(fd[0]);
    close(fd[1]);
    close(fd2[0]);
    waitpid(pid2, NULL, 0);
    printf("Right argument with pipes ahead processing complete.\n");
}

void	simple_pipe_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
    int		fd[2];
    pid_t	pid1;
    pid_t	pid2;

    printf("Starting simple_pipe_redirection.\n");

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return ;
    }
    printf("Pipe created: fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);

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
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);
        printf("Executing first command in simple_pipe_redirection.\n");
        redirect_execve_args(gc, tmp);
        exit(EXIT_FAILURE);
    }
    if (pid1 > 0)
    {
        printf("Parent process: child1 pid: %d\n", pid1);
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
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);
        printf("Executing second command in simple_pipe_redirection.\n");
        redirect_execve_args(gc, head->next);
        exit(EXIT_FAILURE);
    }
    if (pid2 > 0)
    {
        printf("Parent process: child2 pid: %d\n", pid2);
    }
    close(fd[0]);
    waitpid(pid2, NULL, 0);
    printf("simple_pipe_redirection complete.\n");
}
