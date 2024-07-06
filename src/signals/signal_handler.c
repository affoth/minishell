/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/04 17:06:22 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Signal handler for SIGINT
void sigint_handler(int signum)
{
	(void)signum; // Cast to void to suppress unused parameter warning
	if (isatty(STDIN_FILENO))
	{
		// Main shell behavior on SIGINT
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

// Signal handler for SIGQUIT (ignored)
void sigquit_handler(int signum) {
	// Ignore SIGQUIT in heredoc child
	(void)signum;
}

// Function to set up signal handlers in the parent process
void set_signals_parent(void) {
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN); // Optionally handle SIGQUIT
}

// Signal handler for SIGINT in heredoc child process
void sigint_handler_child(int signum)
{
	(void)signum;
}

// Function to set up signal handlers in heredoc child process
void set_signals_child(void) {
	signal(SIGINT, SIG_DFL); // Default behavior for SIGINT in child
	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT in heredoc child
}
