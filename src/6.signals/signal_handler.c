/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/14 22:30:22 by afoth            ###   ########.fr       */
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
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
	perror("sigaction");
	exit(1);
	}
	printf("Child signal setup complete\n");
}


void handle_signal(int sig)
{
	// printf("Parent signal\n");
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
	printf("Child signal\n");
	if (sig == SIGINT)
	{
	// Handle Ctrl-C (SIGINT)
		return;
		//exit(0);
	}
	// Handle Ctrl-\ (SIGQUIT)
	if (sig == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		exit(0);
	}
}
// void	sigint_handler_parent(int num)
// {
// 	(void)num;
// 	write(1, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }



/* void	sigint_handler_child(int num)
{
	(void)num;
	rl_on_new_line();
}

void	sigquit_handler(int num)
{
	(void)num;
}

void	set_signals_parent(void)
{
	signal(SIGINT, sigint_handler_parent);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_child(void)
{
	signal(SIGINT, sigint_handler_child);
	signal(SIGQUIT, sigint_handler_child);
}
signal(SIGQUIT, sigquit_handler);
 */
