/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 17:18:51 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
	//printf("Parent signal setup complete\n");
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
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
/*  printf("Child signal setup complete\n");
 */
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
	{
        return;
	}
	if (sig == SIGPIPE)
	{
		exit(0);
	}
}

void child_handle_signal(int sig)
{
    //printf("Child signal\n");
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
	if (sig == SIGPIPE)
	{
		exit(0);
	}
}

/*
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
 */
