/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 21:48:37 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void setup_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // Set up handlers for SIGINT and SIGQUIT
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return;
    }
	sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return;
    }
    // Ignore SIGPIPE
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(0);
    }
}
void setup_child_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = child_handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // Set up handlers for SIGINT and SIGQUIT
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
    // Ignore SIGPIPE
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(0);
    }
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
    else if (sig == SIGQUIT)
    {
        // Do nothing or handle as needed
    }
    else if (sig == SIGPIPE)
    {
        // Do nothing to ignore SIGPIPE
    }
}

void child_handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        // Handle Ctrl-C (SIGINT)
        return;
    }
    else if (sig == SIGQUIT)
    {
        printf("Quit (core dumped)\n");
        exit(0);
    }
    else if (sig == SIGPIPE)
    {
        // Do nothing to ignore SIGPIPE
    }
}

// Restore original signal handlers after heredoc
void restore_original_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;  // Restore default behavior
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Restore SIGINT and SIGQUIT to default
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}
