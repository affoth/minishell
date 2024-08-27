/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:49:18 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/28 00:03:38 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handle_output_redirection(t_command *current_cmd, t_arg *current_arg) {
    int flags;
    if (current_arg->type == REDIRECTION_OUT) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (current_arg->type == REDIRECTION_APPEND) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else {
        return 0; // Not an output redirection type
    }

    if (current_arg->next) {
        printf("Handling output redirection to file '%s'\n", current_arg->next->arg);
        current_cmd->stdout_fd = open(current_arg->next->arg, flags, 0644);
        if (current_cmd->stdout_fd < 0) {
            perror("Failed to open file for stdout redirection");
        }
        return 1; // Successfully handled the redirection, skip the next arg
    }

    return 0; // No next argument, nothing to handle
}

int handle_input_redirection(t_command *current_cmd, t_arg *current_arg) {
    if (current_arg->type == REDIRECTION_IN) {
        if (current_arg->next) {
            printf("Handling input redirection from file '%s'\n", current_arg->next->arg);
            current_cmd->stdin_fd = open(current_arg->next->arg, O_RDONLY);
            if (current_cmd->stdin_fd < 0) {
                perror("Failed to open file for stdin redirection");
            }
            return 1; // Successfully handled the redirection, skip the next arg
        }
    }
    return 0; // No next argument, nothing to handle
}


/*void handle_heredoc(t_command *current_cmd, t_arg *current_arg) {
    // Implement HEREDOC handling here (e.g., read until a delimiter)
    // This is a placeholder function for future implementation.
}*/
