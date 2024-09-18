/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:29:10 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/18 21:30:37 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static volatile sig_atomic_t heredoc_interrupted = 0;
// Setup signals for heredoc (specifically SIGINT handling)
// Set SIGINT to custom handler (for Ctrl+C)
// Ignore SIGQUIT during heredoc

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}

// Custom signal handler for SIGINT during heredoc
void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		heredoc_interrupted = 1;
		write(STDOUT_FILENO, "\n", 1);
	}
}

int	heredoc(t_shell *shell, const char *delimiter)
{
	int		pipe_fd[2];
	char	*expanded_line;

	setup_heredoc_signals();
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		restore_original_signals();
		return (-1);
	}
	while (1)
	{
		expanded_line = expand_string(shell, readline("heredoc>"), 0);
		if (heredoc_interrupted)
		{
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			restore_original_signals();
			return (-1);
		}
		if (expanded_line == NULL)
			break ;
		if (strcmp(expanded_line, delimiter) == 0)
			break ;
		write(pipe_fd[1], expanded_line, strlen(expanded_line));
		write(pipe_fd[1], "\n", 1);
	}
	close(pipe_fd[1]);
	restore_original_signals();
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
			perror("heredoc");
			return (false);
		}
		cmd->stdin_fd = fd;
		return (true);
	}
	return (false);
}
