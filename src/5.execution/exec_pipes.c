/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:59:22 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 22:04:47 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_file_redirections(t_command *cmd)
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

// If this command is invalid but part of a pipeline, don't exit the pipeline.
			// Just ensure that the command does not execute anything.
pid_t	fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes)
{
	pid_t	pid;
	int		status;


	pid = fork();
	if (pid == 0)
	{
		if (!cmd->valid)
		{
			if (cmd->next != NULL)
			{
				exit(0);
			}
			exit(1);
		}
		setup_redirections(cmd_index, num_pipes, pipe_descriptors);
		close_pipes(num_pipes, pipe_descriptors);
		handle_file_redirections(cmd);
		status = 0;
		if (is_built_in(cmd->cmd_name))
			status = exec_built_ins(shell, cmd);
		else
			status = execute_command(shell, cmd);
		exit(status);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

int	execute_commands_with_pipes(t_shell *shell, t_command *cmds_head)
{
	int			num_pipes;
	int			status;
	t_command	*current_cmd;
	int			cmd_index;
	pid_t		last_pid;
	pid_t		pid;
	int			child_status;



	num_pipes = count_pipes_cmdstruct(cmds_head);
	int	pipe_descriptors[2 * num_pipes];
	status = 0;
	setup_child_signals();
	create_pipes(num_pipes, pipe_descriptors);
	current_cmd = cmds_head;
	cmd_index = 0;
	last_pid = -1;

	while (current_cmd)
	{
		pid = fork_and_execute_command
			(shell, current_cmd, pipe_descriptors, cmd_index, num_pipes);
		if (pid < 0)
			status = 1;
		if (current_cmd->next == NULL)
			last_pid = pid;
		if (cmd_index > 0)
			close(pipe_descriptors[(cmd_index - 1) * 2]);
		if (current_cmd->next)
			close(pipe_descriptors[cmd_index * 2 + 1]);
		current_cmd = current_cmd->next;
		cmd_index++;
	}
	close_pipes(num_pipes, pipe_descriptors);
	if (waitpid(last_pid, &child_status, 0) == -1)
	{
		perror("waitpid");
		status = 1;
	}
	else
	{
		if (WIFEXITED(child_status))
			status = WEXITSTATUS(child_status);
		else if (WIFSIGNALED(child_status))
			status = 128 + WTERMSIG(child_status);
	}

	while (wait(NULL) > 0)
		;
	shell->exit_status = status;
	return (status);
}
