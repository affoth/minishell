/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:29:10 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 22:18:29 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static volatile sig_atomic_t heredoc_interrupted = 0;
// Setup signals for heredoc (specifically SIGINT handling)
void setup_heredoc_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = heredoc_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Set SIGINT to custom handler (for Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    // Ignore SIGQUIT during heredoc
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}
// Custom signal handler for SIGINT during heredoc
void heredoc_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        heredoc_interrupted = 1;
        write(STDOUT_FILENO, "\n", 1); // Print a newline after Ctrl+C
    }
}

int heredoc(t_gc *gc, const char *delimiter)
{
    int pipe_fd[2];
    char *expanded_line;

    // Set up heredoc signal handling
    setup_heredoc_signals();

    // Create a pipe for heredoc input
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        restore_original_signals();
        return -1;
    }

    // Read lines until the delimiter or Ctrl+C
    while (1)
    {
        //expanded_line = readline("heredoc> ");
        expanded_line = expand_string(gc, readline("heredoc>"), 0);
        // If Ctrl+C (SIGINT) was caught
        if (heredoc_interrupted)
        {
            close(pipe_fd[1]); // Close the writing end of the pipe
            close(pipe_fd[0]); // Close the reading end of the pipe
            restore_original_signals();
            return -1; // Indicate heredoc was interrupted
        }

        if (expanded_line == NULL) // Ctrl+D or readline error
        {
            break;
        }

        // If the input expanded_line matches the delimiter, stop reading
        if (strcmp(expanded_line, delimiter) == 0)
        {
            break;
        }

        // Write the line to the pipe
        write(pipe_fd[1], expanded_line, strlen(expanded_line));
        write(pipe_fd[1], "\n", 1); // Add newline
    }

    // Close the writing end of the pipe
    close(pipe_fd[1]);

    // Restore original signal handling before returning
    restore_original_signals();

    // Return the reading end of the pipe for the heredoc
    return pipe_fd[0];
}

// Main function to parse and handle heredoc in a command
bool parse_heredoc(t_gc *gc, t_command *cmd, t_arg *arg)
{
    int fd;

    if (arg->type == HEREDOC)
    {
        const char *delimiter = arg->next->arg;
        fd = heredoc(gc, delimiter);
        if (fd < 0)
        {
            perror("heredoc");
            return false;
        }
        cmd->stdin_fd = fd;
        return true;
    }
    return false;
}
