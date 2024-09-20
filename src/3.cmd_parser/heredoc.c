/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:47:54 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/20 20:16:58 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	delimiter_found(char *line, const char *delimiter)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

int	heredoc(t_shell *shell, const char *delimiter)
{
	int		pipe_fd[2];
	char	*expanded_line;
	char	*line;

	setup_heredoc_signals();
	if (pipe(pipe_fd) == -1)
		return (perror_pipe());
	while (1)
	{
		line = readline("heredoc>");
		if (g_sig == SIGINT)
			return (handle_heredoc_interrupt(shell, pipe_fd, line));
		if (line == NULL)
			break ;
		expanded_line = expand_string(shell, line, 0);
		free(line);
		if (delimiter_found(expanded_line, delimiter) == 0)
			break ;
		write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
		write(pipe_fd[1], "\n", 1);
		free(expanded_line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

// Main function to parse and handle heredoc in a command
bool	parse_heredoc(t_shell *shell, t_command *cmd, t_arg *arg)
{
	int			fd;
	const char	*delimiter;

	if (arg->type == HEREDOC)
	{
		delimiter = arg->next->arg;
		fd = heredoc(shell, delimiter);
		if (fd < 0)
		{
			perror("heredoc interrupted");
			return (false);
		}
		cmd->stdin_fd = fd;
		return (true);
	}
	return (true);
}
