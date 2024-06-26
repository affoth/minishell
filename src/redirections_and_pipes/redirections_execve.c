/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 17:19:55 by afoth             #+#    #+#             */
/*   Updated: 2024/06/26 14:37:02 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// Function to count the number of arguments in args_head
int redirect_count_arguments(t_arg *args_head)
{
	int count = 0;
	while (args_head)
	{
		if (args_head->type == REDIRECTION_IN || args_head->type == REDIRECTION_OUT || args_head->type == REDIRECTION_APPEND || args_head->type == HEREDOC || args_head->type == PIPE)
			return count;
		count++;
		args_head = args_head->next;
	}
	return count;
}

// fork and execve
void redirect_execve_args(t_arg *args_head)
{
	pid_t pid;
	char *path;
	char **args;
	int i;
	int argc;

	argc = redirect_count_arguments(args_head);
	i = 0;
	args = (char **)ft_gc_malloc(sizeof(char *) * (argc + 1));
	while (args_head)
	{
		if (args_head->type == REDIRECTION_IN || args_head->type == REDIRECTION_OUT || args_head->type == REDIRECTION_APPEND || args_head->type == HEREDOC || args_head->type == PIPE)
			break;
		if (args_head->type == WORD || args_head->type == DOUBLE_QUOTED_STRING || args_head->type == SINGLE_QUOTED_STRING)
		{
			args[i] = ft_shell_strdup(args_head->arg);
			i++;
		}
		args_head = args_head->next;
	}
	args[i] = NULL;
	path = get_path(args[0]);
	if (!path)
	{
		ft_printf("NO EXECVE COMMAND FOUND\n");
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, NULL);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
