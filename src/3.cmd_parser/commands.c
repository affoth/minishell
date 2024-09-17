/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:59:47 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 23:07:18 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to create a new command node
t_command	*create_command(t_gc *gc)
{
	t_command *new_cmd;

	new_cmd = (t_command *)ft_gc_malloc(gc, sizeof(t_command));
	if (!new_cmd)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	new_cmd->cmd_name = NULL;
	new_cmd->flags = NULL;
	new_cmd->args = NULL;
	new_cmd->next = NULL;
	new_cmd->stdin_fd = STDIN_FILENO;
	new_cmd->stdout_fd = STDOUT_FILENO;
	new_cmd->append_mode = false;
	new_cmd->valid = true;
	return (new_cmd);
}

int	handle_output_redirection(t_command *cmd, t_arg *arg)
{
	int	fd;

	if (arg->type == REDIRECTION_OUT)
	{
		if (cmd->stdout_fd != STDOUT_FILENO)
		{
			close(cmd->stdout_fd);
		}
		fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			fprintf(stderr, "%s: %s: %s\n",
				cmd->cmd_name ? cmd->cmd_name : "minishell",
				arg->next->arg,
				strerror(errno));
			cmd->valid = false;
			return (1);
		}
		else
			cmd->stdout_fd = fd;
		return (0); // Success
	}
	else if (arg->type == REDIRECTION_APPEND)
	{
		if (cmd->stdout_fd != STDOUT_FILENO)
			close(cmd->stdout_fd);
		fd = open(arg->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			fprintf(stderr, "%s: %s: %s\n",
					cmd->cmd_name ? cmd->cmd_name : "minishell",
					arg->next->arg,
					strerror(errno));
			cmd->valid = false;
			return (1);
		}
		else
		{
			cmd->stdout_fd = fd;
			cmd->append_mode = true;
		}
		return (0);
	}
	return (0);
}

int	handle_input_redirection(t_command *cmd, t_arg *arg)
{
	int	fd;

	if (arg->type == REDIRECTION_IN)
	{
		if (cmd->stdin_fd != STDIN_FILENO)
			close(cmd->stdin_fd);

		fd = open(arg->next->arg, O_RDONLY);
		if (fd < 0)
		{
			fprintf(stderr, "%s: %s: %s\n",
					cmd->cmd_name ? cmd->cmd_name : "minishell",
					arg->next->arg,
					strerror(errno));
			cmd->valid = false;
			return (1);
		}
		else
		{
			cmd->stdin_fd = fd;
		}
		return (0);
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
			close(cmd->stdin_fd);
		}
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
	return (0);
}

// Function to count the number of pipes
int	count_pipes_argstruct(t_arg *args_head)
{
	int		pipe_count;
	t_arg	*current_arg;

	pipe_count = 0;
	current_arg = args_head;

	while (current_arg)
	{
		if (current_arg->type == PIPE)
			pipe_count++;
		current_arg = current_arg->next;
	}
	return (pipe_count);
}

void	add_flag_to_command(t_command *cmd, const char *flag, t_gc *gc)
{
	int		count;
	char	**new_flags;
	int		i;

	count = 0;

	if (cmd->flags)
	{
		while (cmd->flags[count])
			count++;
	}
	new_flags = (char **)ft_gc_malloc(gc, sizeof(char *) * (count + 2));
	if (!new_flags)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < count)
	{
		new_flags[i] = cmd->flags[i];
		i++;
	}
	new_flags[count] = ft_shell_strdup(gc, flag);
	new_flags[count + 1] = NULL;
	if (cmd->flags)
		free(cmd->flags);
	cmd->flags = new_flags;
}

void	add_arg_to_command(t_command *cmd, const char *arg, t_gc *gc)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	new_args = (char **)ft_gc_malloc(gc, sizeof(char *) * (count + 2));
	if (!new_args)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_shell_strdup(gc, arg);
	new_args[count + 1] = NULL;
	cmd->args = new_args;
}

void	set_command_name(t_command *cmd, const char *name, t_gc *gc)
{
	if (cmd->cmd_name == NULL)
		cmd->cmd_name = ft_shell_strdup(gc, name);
}

// Simplified function if `t_command` is not required
char	*strip_redundant_quotes(t_gc *gc, const char *str)
{
	int		len;
	char	*result;
	bool	in_single_quote;
	bool	in_double_quote;
	int		i;
	char	*new_result;

	i = 0;
	len = ft_strlen(str);
	result = ft_shell_strdup(gc, "");
	if (!result)
		return (NULL);
	in_single_quote = false;
	in_double_quote = false;
	while (i < len)
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			continue ;
		}
		if (str[i] == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			continue ;
		}
		char temp[2] = {str[i], '\0'};
		new_result = ft_shell_strjoin(gc, result, temp);
		if (!new_result)
		{
			ft_gc_free(gc);
			return (NULL);
		}
		result = new_result;
		i++;
	}
	return (result);
}

bool	check_files(t_arg *current_arg)
{
	t_arg	*temp;

	temp = current_arg;
	while (temp->type != END && temp->type != PIPE)
	{
		if (temp->type == REDIRECTION_OUT || temp->type == REDIRECTION_APPEND)
		{
			if (access(temp->next->arg, F_OK))
			{
				if (access(temp->next->arg, W_OK))
				{
					//return (false);
				}
			}
		}
		temp = temp->next;
		if (temp == NULL)
			break ;
	}
	return (true);
}

t_command	*create_and_populate_commands(t_shell *shell, t_gc *gc, t_arg *args_head, int pipe_count)
{
	t_command *commands[pipe_count + 1];
	t_command *cmds_head = NULL;
	t_command *last_cmd = NULL;
	t_arg *current_arg = args_head;
	bool open = true;
	int cmd_index = 0;

	// Create command nodes

	int i = 0;
	while (i <= pipe_count)
	{
		commands[i] = create_command(gc);
		if (i == 0)
		{
			cmds_head = commands[i];
		}
		else
		{
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
		if (current_arg->type == REDIRECTION_OUT || current_arg->type == REDIRECTION_APPEND)
		{
			open = check_files(current_arg);
			if (open == true)
			{
				if (handle_output_redirection(current_cmd, current_arg) != 0)
				{
				// Redirection failed; cmd->valid is already set to false
				// Optionally, print additional error messages if needed
				}
		 	   current_arg = current_arg->next->next;
			}
			if (open == false)
			{
				current_cmd->valid = false;
				while (current_arg)
				{
					if (current_arg->type == PIPE)
					{
						current_arg = current_arg->next;
						break;
					}
					current_arg = current_arg->next;
				}
			}

			continue;
		}

		// Handle arguments and flags
		if (current_arg->type == FLAGS) {
			add_flag_to_command(current_cmd, current_arg->arg, gc);
		} else {
			add_arg_to_command(current_cmd, current_arg->arg, gc);
		}
		if (current_arg->type == END) {
			break;
		}
		current_arg = current_arg->next;
	}

	return cmds_head;
}




