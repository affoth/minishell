/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:16:51 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/20 16:47:45 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_command(t_shell *shell, t_command *cmd)
{
	int		flags_count;
	int		args_count;
	char	**args;
	char	*path;

	flags_count = count_flags(cmd);
	args_count = count_args(cmd);
	args = prepare_args(shell, cmd, flags_count, args_count);
	path = get_path(shell, args[0]);
	if (is_directory(args[0]) == 1)
		exit(EXIT_PERMISSION_DENIED);
	if (!path)
	{
		write_error("Command not found: ", args[0]);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	execve(path, args, shell->env);
	perror("execve");
	exit(EXIT_EXECVE_FAILED);
}
