/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:16:51 by mokutucu          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/09/21 18:45:26 by afoth            ###   ########.fr       */
=======
/*   Updated: 2024/09/21 14:02:52 by afoth            ###   ########.fr       */
>>>>>>> 9133e6968902d1243bd25ca601f17eeb5578c339
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
	if (path == NULL || execve(path, args, shell->env) == -1)
	{
<<<<<<< HEAD
=======
		if (path && ft_strncmp(path, args[0], ft_strlen(args[0])))
			free(path);
>>>>>>> 9133e6968902d1243bd25ca601f17eeb5578c339
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	return (EXIT_SUCCESS);
}
