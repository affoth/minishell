/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:12:24 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 20:19:27 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create a new command node
t_command *create_command(t_gc *gc) 
{
    printf("Creating a new command node\n");
    t_command *new_cmd = (t_command *)ft_gc_malloc(gc, sizeof(t_command));
    if (!new_cmd) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_cmd->args_head = NULL;
    new_cmd->next = NULL;
    new_cmd->stdin_fd = STDIN_FILENO;
    new_cmd->stdout_fd = STDOUT_FILENO;
    printf("New command node created successfully\n");
    return new_cmd;
}


// Function to add an argument to a command
void add_arg_to_command(t_command *cmd, t_arg *arg) {
    if (arg == NULL) {
        fprintf(stderr, "Error: arg is NULL\n");
        return;
    }
    if (arg->arg == NULL) {
        fprintf(stderr, "Error: arg->arg is NULL\n");
        return;
    }
    printf("Adding argument '%s' to command\n", arg->arg);
    if (!cmd->args_head) {
        cmd->args_head = arg;
    } else {
        t_arg *current = cmd->args_head;
        while (current->next) {
            current = current->next;
        }
        current->next = arg;
        arg->prev = current;
    }
    printf("Argument '%s' added successfully\n", arg->arg);
}

void print_commands(t_command *cmds_head) {
    if (!cmds_head) {
        printf("No commands to display.\n");
        return;
    }

    t_command *current_cmd = cmds_head;
    int cmd_index = 0;

    while (current_cmd) {
        printf("Command %d:\n", cmd_index);
        t_arg *current_arg = current_cmd->args_head;
        while (current_arg) {
            printf("  Argument: %s, Type: %d\n", current_arg->arg, current_arg->type);
            current_arg = current_arg->next;
        }
        current_cmd = current_cmd->next;
        cmd_index++;
    }
}

int handle_output_redirection(t_command *current_cmd, t_arg *current_arg) {
    int flags;

    if (current_arg->type == REDIRECTION_OUT) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (current_arg->type == REDIRECTION_APPEND) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else {
        return -1; // Not an output redirection type
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

t_command *parse_commands(t_gc *gc, t_arg *args_head) {
    printf("Starting to parse commands\n");
    t_command *cmds_head = NULL;
    t_command *current_cmd = NULL;
    t_arg *current_arg = args_head;

    while (current_arg) {
        printf("Processing argument: %s, Type: %d\n", current_arg->arg, current_arg->type);
        if (current_arg->type == PIPE) {
            printf("Encountered PIPE, setting current_cmd to NULL\n");
            current_cmd = NULL;
        } else {
            if (!current_cmd) {
                printf("Creating a new command because current_cmd is NULL\n");
                current_cmd = create_command(gc);
                if (!cmds_head) {
                    cmds_head = current_cmd;
                } else {
                    t_command *last_cmd = cmds_head;
                    while (last_cmd->next) {
                        last_cmd = last_cmd->next;
                    }
                    last_cmd->next = current_cmd;
                }
            }

            int skip_next_arg = 0;

            // Handle redirection logic in separate functions
            skip_next_arg = handle_output_redirection(current_cmd, current_arg);
            if (!skip_next_arg) {
                skip_next_arg = handle_input_redirection(current_cmd, current_arg);
            }
            /*if (!skip_next_arg && current_arg->type == HEREDOC) {
                handle_heredoc(current_cmd, current_arg);
            }*/

            // If no redirection was handled, treat it as a normal argument
            if (!skip_next_arg) {
                add_arg_to_command(current_cmd, current_arg);
            } else {
                current_arg = current_arg->next; // Skip the next argument
                printf("Skipping the next argument\n");
            }
        }

        current_arg = current_arg->next;
    }
    print_commands(cmds_head);

    printf("Finished parsing commands\n");
    return cmds_head;
}