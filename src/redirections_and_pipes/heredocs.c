/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:59:16 by afoth             #+#    #+#             */
/*   Updated: 2024/06/27 16:02:14 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	heredoc(t_arg *head, t_arg *tmp)
{
	int		fd[2];
	pid_t	pid;
	char	*line;

	if (pipe(fd) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = readline("> ");
			if (line == NULL)
			{
				printf("exit\n");
				exit(0);
			}
			if (ft_strncmp(line, head->next->arg, ft_strlen(head->next->arg)) == 0)
			{
				free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		exit(0);
	}
	else
	{
		waitpid(pid, NULL, 0);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		redirect_execve_args(tmp);
	}
}
