/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:29:10 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/04 16:29:20 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool parse_heredoc(t_command *cmd, t_arg *arg) {
    if (arg->type == HEREDOC) {
        const char *delimiter = arg->next->arg;
        char *heredoc_content = NULL;
        size_t content_size = 0;
        char buffer[1024];

        printf("Enter heredoc content (end with %s):\n", delimiter);

        while (fgets(buffer, sizeof(buffer), stdin)) {
            if (strncmp(buffer, delimiter, strlen(delimiter)) == 0) {
                break;
            }
            size_t buffer_len = strlen(buffer);
            heredoc_content = realloc(heredoc_content, content_size + buffer_len + 1);
            memcpy(heredoc_content + content_size, buffer, buffer_len);
            content_size += buffer_len;
        }

        if (heredoc_content) {
            heredoc_content[content_size] = '\0';
            // Save heredoc_content to a temporary file
            char temp_file[] = "/tmp/heredocXXXXXX";
            int fd = mkstemp(temp_file);
            if (fd < 0) {
                perror("mkstemp");
                return true;
            }
            write(fd, heredoc_content, content_size);
            close(fd);

            // Set the heredoc file descriptor for input redirection
            cmd->stdin_fd = open(temp_file, O_RDONLY);
            if (cmd->stdin_fd < 0) {
                perror("Error opening heredoc file");
                return true;
            }
            free(heredoc_content);
        }
        return true;
    }
    return false;
}
