/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:29:10 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/15 13:23:51 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			//IS THIS CORRECT???
			return false;
		}
		//IS THIS CORRECT???
    	cmd->stdin_fd = fd;

        if (cmd->stdin_fd < 0)
		{
            perror("Error opening heredoc file");
			//IS THIS CORRECT???
            return true;
        }
        return true;
    }

    return false;
}


int heredoc(const char *delimiter)
{
	int pipe_fd[2];
	pid_t child_pid;
	char *line = NULL;
	int status;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}

	setup_child_signals();
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return(-1);
	}
	//CHECK THIS!!!
	else if (child_pid == 0)
	{
		// Child process to handle Here Document input
		close(pipe_fd[0]); // Close reading end of the pipe

		while (1)
		{
			line = readline("heredoc> ");
			if (line == NULL) // Ctrl+D or readline error
				break;

			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			write(pipe_fd[1], line, strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}

		return(pipe_fd[1]);
	}
	else
	{
		// Parent process
		//THIS CORRECT???
		return(pipe_fd[1]);

		// Wait for the child process to complete
		waitpid(child_pid, &status, 0);

		// Redirect standard input to the pipe if child process completed normally
		if (WIFEXITED(status)) {
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
				perror("dup2");
				close(pipe_fd[0]);
				return (-1);
			}
			close(pipe_fd[0]);
		}
	}
	return(-1);
}






/* bool parse_heredoc(t_command *cmd, t_arg *arg)
{
    if (arg->type == HEREDOC)
	{
        const char *delimiter = arg->next->arg;
        char *heredoc_content = NULL;
        size_t content_size = 0;
        char buffer[1024];

        printf("Enter heredoc content (end with %s):\n", delimiter);

        while (fgets(buffer, sizeof(buffer), stdin))
		{
            if (strncmp(buffer, delimiter, strlen(delimiter)) == 0)
			{
                break;
            }

            size_t buffer_len = strlen(buffer);
            heredoc_content = realloc(heredoc_content, content_size + buffer_len + 1);
            memcpy(heredoc_content + content_size, buffer, buffer_len);
            content_size += buffer_len;
        }

        if (heredoc_content)
		{
            heredoc_content[content_size] = '\0';
            // Save heredoc_content to a temporary file
            char temp_file[] = "/tmp/heredocXXXXXX";
            int fd = mkstemp(temp_file);
           if (fd < 0)
		   {
                perror("mkstemp");
                return true;
            }
            write(fd, heredoc_content, content_size);
            close(fd);

            // Set the heredoc file descriptor for input redirection
            cmd->stdin_fd = open(temp_file, O_RDONLY);
            if (cmd->stdin_fd < 0)
			{
                perror("Error opening heredoc file");
                return true;
            }
            free(heredoc_content);
        }
        return true;
    }
    return false;
} */
