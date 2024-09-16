/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:59:47 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 23:02:57 by mokutucu         ###   ########.fr       */
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
    new_cmd->valid = true; // Initialize as valid
    return new_cmd;
}

int handle_output_redirection(t_command *cmd, t_arg *arg) {
    if (arg->type == REDIRECTION_OUT) {
        // Close previous output file descriptor if already redirected
        if (cmd->stdout_fd != STDOUT_FILENO) {
            close(cmd->stdout_fd);
        }

        int fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            // Print error message
            fprintf(stderr, "%s: %s: %s\n",
                    cmd->cmd_name ? cmd->cmd_name : "minishell",
                    arg->next->arg,
                    strerror(errno));
            cmd->valid = false; // Mark command as invalid
            return 1; // Return failure
        } else {
            cmd->stdout_fd = fd;
            // Do NOT reset cmd->valid to true here
        }
        return 0; // Success
    } else if (arg->type == REDIRECTION_APPEND) {
        // Handle append mode
        if (cmd->stdout_fd != STDOUT_FILENO) {
            close(cmd->stdout_fd);
        }

        int fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            // Print error message
            fprintf(stderr, "%s: %s: %s\n",
                    cmd->cmd_name ? cmd->cmd_name : "minishell",
                    arg->next->arg,
                    strerror(errno));
            cmd->valid = false; // Mark command as invalid
            return 1; // Return failure
        } else {
            cmd->stdout_fd = fd;
            cmd->append_mode = true;
            // Do NOT reset cmd->valid to true here
        }
        return 0; // Success
    }
    return 0;
}

int handle_input_redirection(t_command *cmd, t_arg *arg) {
    if (arg->type == REDIRECTION_IN)
	{
        // Close previous input file descriptor if already redirected
        if (cmd->stdin_fd != STDIN_FILENO)
		{
            close(cmd->stdin_fd);
        }

        int fd = open(arg->next->arg, O_RDONLY);
        if (fd < 0)
		{
            // Print error message
            fprintf(stderr, "%s: %s: %s\n",
                    cmd->cmd_name ? cmd->cmd_name : "minishell",
                    arg->next->arg,
                    strerror(errno));
            cmd->valid = false; // Mark command as invalid
            return 1; // Return failure
        }
		else
		{
            cmd->stdin_fd = fd;
            // Do NOT reset cmd->valid to true here
        }
        return 0; // Success
    }
	if (arg->type == HEREDOC)
	{
	    if (cmd->stdin_fd != STDIN_FILENO)
		{
        	if (dup2(cmd->stdin_fd, STDIN_FILENO) < 0)
        	{
            	perror("dup2 stdin_fd");
            	exit(EXIT_FAILURE);
        	}
        	close(cmd->stdin_fd);  // Close the original fd after redirection
    	}

    // Redirect stdout similarly if necessary
    	if (cmd->stdout_fd != STDOUT_FILENO)
    	{
    	    if (dup2(cmd->stdout_fd, STDOUT_FILENO) < 0)
    	    {
    	        perror("dup2 stdout_fd");
    	        exit(EXIT_FAILURE);
    	    }
    	    close(cmd->stdout_fd);
    	}
	}
    return 0;
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

// Simplified function if `t_command` is not required
char *strip_redundant_quotes(t_gc *gc, const char *str)
{
    int len = ft_strlen(str);
    char *result = ft_shell_strdup(gc, "");  // Start with an empty string

    if (!result)
        return NULL;

    bool in_single_quote = false;
    bool in_double_quote = false;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            continue;
        }
        if (str[i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            continue;
        }
        char temp[2] = {str[i], '\0'};
        char *new_result = ft_shell_strjoin(gc, result, temp);
        if (!new_result)
        {
            ft_gc_free(gc);  // Ensure no memory leaks
            return NULL;
        }
        result = new_result;
    }

    return result;
}

t_command *create_and_populate_commands(t_shell *shell, t_gc *gc, t_arg *args_head, int pipe_count) {
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
        if (parse_heredoc(shell, current_cmd, current_arg)) {
            current_arg = current_arg->next->next;
            continue;
        }

        // Handle command name (first argument)
        if (current_cmd->cmd_name == NULL && current_arg->type == WORD) {
            set_command_name(current_cmd, current_arg->arg, gc);
            current_arg = current_arg->next;
            continue;
        }

        // Handle redirections
        if (current_arg->type == REDIRECTION_IN) {
            if (handle_input_redirection(current_cmd, current_arg) != 0) {
                // Redirection failed; cmd->valid is already set to false
                // Optionally, print additional error messages if needed
            }
            current_arg = current_arg->next->next;
            continue;
        }
        if (current_arg->type == REDIRECTION_OUT || current_arg->type == REDIRECTION_APPEND) {
            if (handle_output_redirection(current_cmd, current_arg) != 0) {
                // Redirection failed; cmd->valid is already set to false
                // Optionally, print additional error messages if needed
            }
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
