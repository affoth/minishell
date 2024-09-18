/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:25:51 by afoth             #+#    #+#             */
/*   Updated: 2024/09/18 21:11:16 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	check_if_command_is_valid(t_command *cmd)
{
	if (!cmd->valid)
	{
		if (cmd->next != NULL)
			exit(0);
		exit(1);
	}
}

int	count_flags(t_command *cmd)
{
	int	flag_count;

	flag_count = 0;
	while (cmd->flags && cmd->flags[flag_count])
		flag_count++;
	return (flag_count);
}

int	count_args(t_command *cmd)
{
	int	args_count;

	args_count = 0;
	while (cmd->args && cmd->args[args_count])
		args_count++;
	return (args_count);
}
