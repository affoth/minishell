/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:10:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/22 15:40:54 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void heredoc(const char *delimiter)
{
	int pipe_fd[2];
	pid_t child_pid;
	char *line = NULL;
	int status;

	if (pipe(pipe_fd) == -1) {
		perror("pipe");
		return;
	}

	child_pid = fork();
	if (child_pid == -1) {
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return;
	}
	else if (child_pid == 0)
	{
		// Child process to handle Here Document input
		close(pipe_fd[0]); // Close reading end of the pipe
		set_signals_child();

		while (1)
		{
			line = readline("heredoc> ");
			if (line == NULL || strcmp(line, delimiter) == 0)
				break;

			write(pipe_fd[1], line, strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}

		free(line);
		close(pipe_fd[1]);
		exit(0);
	}
	else
	{
		// Parent process
		close(pipe_fd[1]); // Close writing end of the pipe

		// Wait for the child process to complete
		waitpid(child_pid, &status, 0);

		if (WIFEXITED(status)) {
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
				perror("dup2");
				close(pipe_fd[0]);
				return;
			}
			close(pipe_fd[0]);
		}
	}
}

