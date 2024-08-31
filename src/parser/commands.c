/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:12:24 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 13:23:58 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create a new command node
t_command *create_command(t_gc *gc) 
{
    t_command *new_cmd = (t_command *)ft_gc_malloc(gc, sizeof(t_command));
    if (!new_cmd) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_cmd->cmd_name = NULL; // Initialize cmd_name as NULL
    new_cmd->flags = NULL;    // Initialize flags as NULL
    new_cmd->args = NULL;     // Initialize args as NULL
    new_cmd->next = NULL;
    new_cmd->stdin_fd = STDIN_FILENO;
    new_cmd->stdout_fd = STDOUT_FILENO;
    new_cmd->append_mode = false;
    return new_cmd;
}

// Function to handle output redirection
bool handle_output_redirection(t_command *cmd, t_arg *arg) {
    if (arg->type == REDIRECTION_OUT) {
        cmd->stdout_fd = open(arg->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->stdout_fd < 0) {
            perror("Error opening file for output redirection");
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
            perror("Error opening file for input redirection");
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

// Add an argument to the command's flags array
void add_flag_to_command(t_command *cmd, const char *flag, t_gc *gc) {
    char **new_flags;
    int i = 0;

    // Allocate space for new flags array
    if (cmd->flags == NULL) {
        cmd->flags = (char **)ft_gc_malloc(gc, sizeof(char *));
        cmd->flags[0] = ft_shell_strdup(gc, flag);
        cmd->flags[1] = NULL; // NULL-terminated
    } else {
        // Count existing flags
        while (cmd->flags[i] != NULL) {
            i++;
        }
        new_flags = (char **)ft_gc_malloc(gc, (i + 2) * sizeof(char *));
        for (int j = 0; j < i; j++) {
            new_flags[j] = cmd->flags[j];
        }
        new_flags[i] = ft_shell_strdup(gc, flag);
        new_flags[i + 1] = NULL;
        cmd->flags = new_flags;
    }
}

// Add an argument to the command's args array
void add_arg_to_command(t_command *cmd, const char *arg, t_gc *gc) {
    char **new_args;
    int i = 0;

    // Allocate space for new args array
    if (cmd->args == NULL) {
        cmd->args = (char **)ft_gc_malloc(gc, sizeof(char *));
        cmd->args[0] = ft_shell_strdup(gc, arg);
        cmd->args[1] = NULL; // NULL-terminated
    } else {
        // Count existing args
        while (cmd->args[i] != NULL) {
            i++;
        }
        new_args = (char **)ft_gc_malloc(gc, (i + 2) * sizeof(char *));
        for (int j = 0; j < i; j++) {
            new_args[j] = cmd->args[j];
        }
        new_args[i] = ft_shell_strdup(gc, arg);
        new_args[i + 1] = NULL;
        cmd->args = new_args;
    }
}

// Set the command name
void set_command_name(t_command *cmd, const char *name, t_gc *gc) {
    cmd->cmd_name = ft_shell_strdup(gc, name);
}

// Function to create and populate commands from argument list
t_command *create_and_populate_commands(t_gc *gc, t_arg *args_head, int pipe_count) {
    t_command *commands[pipe_count + 1];
    t_command *cmds_head = NULL;
    t_command *last_cmd = NULL;
    t_arg *current_arg = args_head;
    int cmd_index = 0;

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
    t_command *current_cmd = cmds_head;
    while (current_arg) {
        if (current_arg->type == PIPE) {
            if (cmd_index < pipe_count) {
                current_cmd = commands[++cmd_index];
            }
            current_arg = current_arg->next;
            continue;
        }

        if (current_arg->type == END) {
            break;
        }

        // Handle the command name, which should be the first argument
        if (current_cmd->cmd_name == NULL && current_arg->type == WORD) {
            set_command_name(current_cmd, current_arg->arg, gc);
            current_arg = current_arg->next;
            continue; // Move to the next argument
        }

        // Handle redirections and arguments
        if (!handle_output_redirection(current_cmd, current_arg) &&
            !handle_input_redirection(current_cmd, current_arg)) {
            if (current_arg->type == REDIRECTION_APPEND) {
                current_cmd->append_mode = true;
            } else if (current_arg->type == FLAGS) {
                add_flag_to_command(current_cmd, current_arg->arg, gc);
            } else {
                add_arg_to_command(current_cmd, current_arg->arg, gc);
            }
        }

        current_arg = current_arg->next;
    }

    return cmds_head;
}
