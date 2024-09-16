/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:29:10 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 21:09:41 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int heredoc(const char *delimiter)
{
    int pipe_fd[2];
    char *line = NULL;

    // Create a pipe for inter-process communication
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }

    while (1)
    {
        line = readline("heredoc> ");
        if (line == NULL) // Ctrl+D or readline error
            break;

        // If the input line matches the delimiter, stop reading
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        // Write the line to the pipe
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1); // Add newline
        free(line);
    }

    // Close the writing end of the pipe
    close(pipe_fd[1]);

    // Return the reading end of the pipe
    return pipe_fd[0];
}

// Function to write heredoc content to a file for testing
bool write_heredoc_to_file(int fd, const char *filename)
{
    int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1)
    {
        perror("open file.txt");
        close(fd);
        return false;
    }

    char buffer[1024];
    ssize_t bytes_read;

    // Read from the heredoc pipe and write to the file
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(file_fd, buffer, bytes_read);
    }

    close(file_fd);
    close(fd); // Close the reading end of the pipe

    return true;
}

// Main function to parse and handle heredoc in a command
bool parse_heredoc(t_command *cmd, t_arg *arg)
{
    int fd;

    if (arg->type == HEREDOC)
    {
        const char *delimiter = arg->next->arg;
        fd = heredoc(delimiter);
        if (fd < 0)
        {
            perror("heredoc");
            return false;
        }
/*
        // Redirect stdin to the heredoc pipe
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return false;
        }*/
      // Write heredoc content to a file for testing
        // if (!write_heredoc_to_file(fd, "file.txt"))
        // {
        //     return false;
        // }
        cmd->stdin_fd = fd;
        return true;
    }
    return false;
}
