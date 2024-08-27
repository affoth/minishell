/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:12:24 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/28 00:03:25 by mokutucu         ###   ########.fr       */
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
    if (!arg || !arg->arg) {
        fprintf(stderr, "Error: invalid argument\n");
        return;
    }

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

// Function to parse commands from the tokenized arguments
t_command *parse_commands(t_gc *gc, t_arg *args_head) {
    printf("Starting to parse commands\n");
    t_command *cmds_head = NULL;
    t_command *current_cmd = NULL;
    t_arg *current_arg = args_head;
    t_command *last_cmd = NULL;

    while (current_arg) {
        printf("Processing argument: %s, Type: %d\n", current_arg->arg, current_arg->type);

        if (current_arg->type == PIPE) {
            printf("Encountered PIPE\n");
            current_cmd = NULL; // This will ensure the next command starts fresh
        } else {
            if (!current_cmd) {
                printf("Creating a new command because current_cmd is NULL\n");
                current_cmd = create_command(gc);
                if (!cmds_head) {
                    cmds_head = current_cmd;
                } else {
                    last_cmd->next = current_cmd;
                }
                last_cmd = current_cmd;
            }

            // Handle redirection and add arguments
            if (!handle_output_redirection(current_cmd, current_arg) &&
                !handle_input_redirection(current_cmd, current_arg)) {
                add_arg_to_command(current_cmd, current_arg);
            }

            // Skip next argument if redirection was handled
            if (current_arg->type == REDIRECTION_OUT || current_arg->type == REDIRECTION_IN) {
                current_arg = current_arg->next;
            }
        }

        current_arg = current_arg->next;
    }
    print_commands(cmds_head);

    printf("Finished parsing commands\n");
    return cmds_head;
}
