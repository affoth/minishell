/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/10 21:39:19 by afoth            ###   ########.fr       */
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
	printf("Setting up child signal handlers in PID: %d\n", getpid());

	sa.sa_handler = child_handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction (child SIGINT)");
        exit(1);
    }
    else
    {
        printf("Child process: SIGINT handler set\n");
    }

    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction (child SIGQUIT)");
        exit(1);
    }
    else
    {
        printf("Child process: SIGQUIT handler set\n");
    }
}


void handle_signal(int sig)
{
	write(STDOUT_FILENO, "parent signal\n", 14);
	if (sig == SIGINT)
	{
	// Handle Ctrl-C (SIGINT)
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "USE CTRL-D TO EXIT\n", 20);
		return;
	}
}
void child_handle_signal(int sig)
{
	write(STDOUT_FILENO, "child signal\n", 13);
	if (sig == SIGINT)
	{
	// Handle Ctrl-C (SIGINT)
		exit(0);
	}
	if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		exit(0);
		//exit(0);
	}
}
/* void handle_eof(void) {
    if (isatty(STDIN_FILENO)) {
    write(STDOUT_FILENO, "exit\n", 5);
    exit(0);
    }
} */

/* void sigint_handler(int signum)
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
 */
