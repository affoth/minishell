/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arg_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:54:02 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 18:13:05 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	if_redir(t_arg *current_arg, t_command *current_cmd)
{
	if (current_arg->type == REDIRECTION_OUT
		|| current_arg->type == REDIRECTION_APPEND)
	{
		if (handle_output_redirection(current_cmd, current_arg) != 0)
		{
		}
		return (0);
	}
	if (current_arg->type == REDIRECTION_IN)
	{
		if (handle_input_redirection(current_cmd, current_arg) != 0)
		{
		}
		return (0);
	}
	return (1);
}

void	handle_arguments(t_command *cmd, t_arg *arg, t_gc *gc)
{
	if (arg->type == FLAGS)
	{
		add_flag_to_command(cmd, arg->arg, gc);
	}
	else
	{
		add_arg_to_command(cmd, arg->arg, gc);
	}
}

int	check_heredoc(t_shell *shell, t_command *cmd, t_arg *arg)
{
	int	state;

	if (arg->type != HEREDOC)
		return (0);
	state = parse_heredoc(shell, cmd, arg);
	if (state == false)
		return (1);
	arg = arg->next->next;
	return (0);
}

int	handle_arg(t_shell *shell, t_command *cmd, t_arg **arg, t_gc *gc)
{
	if ((*arg)->type == PIPE)
	{
		*arg = (*arg)->next;
		cmd = cmd->next;
	}
	else if (check_heredoc(shell, cmd, *arg) == 1)
		return (1);
	else if (cmd->cmd_name == NULL && (*arg)->type == WORD)
	{
		set_command_name(cmd, (*arg)->arg, gc);
		*arg = (*arg)->next;
	}
	else if (if_redir(*arg, cmd) == 0)
		*arg = (*arg)->next->next;
	else
	{
		handle_arguments(cmd, *arg, gc);
		*arg = (*arg)->next;
	}
	return (0);
}
