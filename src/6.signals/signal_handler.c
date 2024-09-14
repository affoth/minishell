/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/14 21:05:11 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Ctrl-C (SIGINT): Interrupts the current command and returns to the prompt.
Ctrl-D (EOF): Exits the shell if the input is empty.
Ctrl-\ (SIGQUIT): Typically causes the process to quit and dump core,
but in interactive shells, it is often ignored.
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
•In interactive mode:
◦ctrl-C displays a new prompt on a new line.
◦ctrl-D exits the shell.
◦ctrl-\ does nothing.
 */
// Signal handler for SIGINT
// Signal handler function
void setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
	perror("sigaction");
	return;
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
	perror("sigaction");
	return;
	}
}
void setup_child_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = child_handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
	perror("sigaction");
	exit(1);
	}
	// if (sigaction(SIGQUIT, &sa, NULL) == -1)
	// {
	// perror("sigaction");
	// exit(1);
	// }
}


void handle_signal(int sig)
{
	if (sig == SIGINT)
	{
	// Handle Ctrl-C (SIGINT)
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	}
	if (sig == SIGQUIT)
		return;
}
void child_handle_signal(int sig)
{
	if (sig == SIGINT)
	{
	// Handle Ctrl-C (SIGINT)
		exit(0);
	}
// 	if (sig == SIGQUIT)
// 		exit(0);
}
