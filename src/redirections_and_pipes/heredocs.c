/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:10:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/01 19:53:21 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to handle Here Document
void heredoc(const char *delimiter)
{
    int fd[2];
    pid_t pid;
    char *line = NULL;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return; // Exit function on pipe creation failure
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return; // Exit function on fork failure
    }
    else if (pid == 0)
    {
        // Child process to write Here Document content to the pipe
        close(fd[0]); // Close reading end of the pipe

        // Use readline for interactive input handling
        while ((line = readline("heredoc> ")) != NULL)
        {
            if (strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            write(fd[1], line, strlen(line));
            write(fd[1], "\n", 1);
            free(line);
        }

        close(fd[1]);
        // exit(EXIT_SUCCESS); // Exit child process after completing Here Document
    }
    else
    {
        // Parent process
        close(fd[1]); // Close writing end of the pipe
        waitpid(pid, NULL, 0); // Wait for child process to complete

        // Redirect the standard input to the pipe
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd[0]);
            return; // Exit function on dup2 failure
        }
        close(fd[0]);
    }
}
