/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:16:51 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 22:43:38 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_command(t_shell *shell, t_command *cmd)
{
	int		flags_count;
	int		args_count;
	char	**args;
	int		i;
	int		j;
	char	*path;


	flags_count = 0;
	while (cmd->flags && cmd->flags[flags_count])
		flags_count++;
	args_count = 0;
	while (cmd->args && cmd->args[args_count])
		args_count++;
	args = (char **)ft_gc_malloc
		(&shell->gc, sizeof(char *) * (flags_count + args_count + 2));
	if (!args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < flags_count)
	{
		args[i + 1] = ft_shell_strdup(&shell->gc, cmd->flags[i]);
		if (!args[i + 1])
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	args[0] = ft_shell_strdup(&shell->gc, cmd->cmd_name);
	if (!args[0])
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	j = 0;
	while (j < args_count)
	{
		args[flags_count + 1 + j] = remove_quotes
			(&shell->gc, strdup(cmd->args[j]));
		if (!args[flags_count + 1 + j])
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		j++;
	}
	args[flags_count + args_count + 1] = NULL;
	path = get_path(&shell->gc, args[0]);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	execve(path, args, shell->env);
	perror("execve");
	exit(EXIT_EXECVE_FAILED);
}

