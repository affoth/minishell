/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:39:26 by afoth             #+#    #+#             */
/*   Updated: 2024/09/20 20:25:57 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	handle_heredoc_interrupt(t_shell *shell, int pipe_fd[2], char *line)
{
	shell->exit_status = 130;
	g_sig = 0;
	free(line);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	return (-1);
}

// Custom signal handler for SIGINT during heredoc
void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = sig;
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}
