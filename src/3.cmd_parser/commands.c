/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:59:47 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/11 20:41:53 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create a new command node
t_command *create_command(t_gc *gc) {
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
		cmd->stdout_fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->stdout_fd < 0) {
			perror("Error opening file for output redirection");
			printf("Failed to open file: %s\n", arg->next->arg);
			return true;
		}
		return true;
	} else if (arg->type == REDIRECTION_APPEND) {
		cmd->stdout_fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->stdout_fd < 0) {
			perror("Error opening file for output redirection");
			return true;
		}
		cmd->append_mode = true;
		return true;
	}
	return false;
}

// Function to handle input redirection
bool handle_input_redirection(t_command *cmd, t_arg *arg) {
	if (arg->type == REDIRECTION_IN) {
		cmd->stdin_fd = open(arg->next->arg, O_RDONLY);
		if (cmd->stdin_fd < 0) {
			perror("Error opening file for input redirection");
			return true;
		}
		return true;
	}
	return false;
}

// Function to count the number of pipes
int count_pipes_argstruct(t_arg *args_head) {
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

void add_flag_to_command(t_command *cmd, const char *flag, t_gc *gc) {
	int count = 0;

	// Count existing flags
	if (cmd->flags) {
		while (cmd->flags[count]) {
			count++;
		}
	}

	// Allocate space for new flags array
	char **new_flags = (char **)ft_gc_malloc(gc, sizeof(char *) * (count + 2));
	if (!new_flags) {
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	// Copy existing flags
	int i = 0;
	while (i < count) {
		new_flags[i] = cmd->flags[i];
		i++;
	}
	new_flags[count] = ft_shell_strdup(gc, flag);
	new_flags[count + 1] = NULL;

	// Free old flags array and assign new one
	if (cmd->flags) {
		free(cmd->flags);
	}
	cmd->flags = new_flags;
}

void add_arg_to_command(t_command *cmd, const char *arg, t_gc *gc) {
	int count = 0;

	// Count existing args
	if (cmd->args) {
		while (cmd->args[count]) {
			count++;
		}
	}

	// Allocate space for new args array
	char **new_args = (char **)ft_gc_malloc(gc, sizeof(char *) * (count + 2));
	if (!new_args) {
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	// Copy existing args
	int i = 0;
	while (i < count) {
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_shell_strdup(gc, arg);
	new_args[count + 1] = NULL;

	cmd->args = new_args;
}

void set_command_name(t_command *cmd, const char *name, t_gc *gc) {
	if (cmd->cmd_name == NULL) {  // Ensure it's only set if not already set
		cmd->cmd_name = ft_shell_strdup(gc, name);
	}
}

t_command *create_and_populate_commands(t_gc *gc, t_arg *args_head, int pipe_count) {
	t_command *commands[pipe_count + 1];
	t_command *cmds_head = NULL;
	t_command *last_cmd = NULL;
	t_arg *current_arg = args_head;
	int cmd_index = 0;

	// Create command nodes
	int i = 0;
	while (i <= pipe_count) {
		commands[i] = create_command(gc);
		if (i == 0) {
			cmds_head = commands[i];
		} else {
			last_cmd->next = commands[i];
		}
		last_cmd = commands[i];
		i++;
	}

	// Populate commands with arguments
	t_command *current_cmd = cmds_head;
	while (current_arg) {
		// Handle pipes by moving to the next command
		if (current_arg->type == PIPE) {
			if (cmd_index < pipe_count) {
				current_cmd = commands[++cmd_index];
			}
			current_arg = current_arg->next;
			continue;
		}

		// Stop processing if END is encountered
		if (current_arg->type == END) {
			break;
		}

		  // Handle heredocs
        if (parse_heredoc(current_cmd, current_arg)) {
            current_arg = current_arg->next;
            continue;
        }

		// Handle command name (first argument)
		if (current_cmd->cmd_name == NULL && current_arg->type == WORD) {
			set_command_name(current_cmd, current_arg->arg, gc);
			current_arg = current_arg->next;
			continue;
		}

		// Handle redirections
		if (handle_output_redirection(current_cmd, current_arg) ||
			handle_input_redirection(current_cmd, current_arg)) {
			current_arg = current_arg->next->next;
			continue;
		}

		// Handle arguments and flags
		if (current_arg->type == FLAGS) {
			add_flag_to_command(current_cmd, current_arg->arg, gc);
		} else {
			add_arg_to_command(current_cmd, current_arg->arg, gc);
		}

		current_arg = current_arg->next;
	}

	return cmds_head;
}
