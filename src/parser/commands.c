/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:12:24 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/29 17:22:23 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create a new command node
t_command *create_command(t_gc *gc) 
{
    t_command *new_cmd = (t_command *)ft_gc_malloc(gc, sizeof(t_command));
    if (!new_cmd) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_cmd->cmd_name = NULL;
    new_cmd->args_head = NULL;
    new_cmd->args_tail = NULL;
    new_cmd->next = NULL;
    new_cmd->stdin_fd = STDIN_FILENO;
    new_cmd->stdout_fd = STDOUT_FILENO;
    new_cmd->append_mode = false;
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
        cmd->args_tail = arg;
    } else {
        cmd->args_tail->next = arg;
        arg->prev = cmd->args_tail;
        cmd->args_tail = arg;
    }
}

// Function to handle output redirection
bool handle_output_redirection(t_command *cmd, t_arg *arg) {
    if (arg->type == REDIRECTION_OUT) {
        cmd->stdout_fd = open(arg->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->stdout_fd < 0) {
            perror("open");
            return true;
        }
        return true;
    }
    return false;
}

// Function to handle input redirection
bool handle_input_redirection(t_command *cmd, t_arg *arg) {
    if (arg->type == REDIRECTION_IN) {
        cmd->stdin_fd = open(arg->arg, O_RDONLY);
        if (cmd->stdin_fd < 0) {
            perror("open");
            return true;
        }
        return true;
    }
    return false;
}

// Function to count the number of pipes
int count_pipes(t_arg *args_head) {
    int pipe_count = 0;
    t_arg *current_arg = args_head;

    while (current_arg) {
        if (current_arg->type == PIPE) {
            pipe_count++;
        }
        current_arg = current_arg->next;
    }
    return pipe_count;
}

// Function to create and populate commands
t_command *create_and_populate_commands(t_gc *gc, t_arg *args_head, int pipe_count) {
    t_command *cmds_head = NULL;
    t_command *current_cmd = NULL;
    t_command *last_cmd = NULL;
    t_arg *current_arg = args_head;
    int cmd_index = 0;

    // Create an array of command pointers
    t_command **commands = (t_command **)ft_gc_malloc(gc, (pipe_count + 1) * sizeof(t_command *));
    if (!commands) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Create commands
    for (int i = 0; i <= pipe_count; i++) {
        commands[i] = create_command(gc);
        if (i == 0) {
            cmds_head = commands[i];
        } else {
            last_cmd->next = commands[i];
        }
        last_cmd = commands[i];
    }

    // Populate commands with arguments
    current_cmd = cmds_head;
    while (current_arg) {
        if (current_arg->type == PIPE) {
            // Move to the next command without adding the PIPE as an argument
            if (cmd_index < pipe_count) {
                current_cmd = commands[++cmd_index];
            }
            // Move past the PIPE token
            current_arg = current_arg->next;
            continue;
        }

        if (current_arg->type == END) {
            break;
        }

        // Handle redirection and append modes
        if (!handle_output_redirection(current_cmd, current_arg) &&
            !handle_input_redirection(current_cmd, current_arg)) {
            // Add argument to the current command, skipping PIPE
            add_arg_to_command(current_cmd, current_arg);
        }

        if (current_arg->type == REDIRECTION_APPEND) {
            current_cmd->append_mode = true;
        }

        // Move to the next argument
        current_arg = current_arg->next;
    }

    free(commands);
    return cmds_head;
}

