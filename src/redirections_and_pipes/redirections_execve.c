/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 17:19:55 by afoth             #+#    #+#             */
/*   Updated: 2024/07/01 15:49:56 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to count the number of arguments in args_head
int redirect_count_arguments(t_arg *args_head)
{
	int count = 0;
	while (args_head)
	{
		if (args_head->type == REDIRECTION_IN || args_head->type == REDIRECTION_OUT ||
			args_head->type == REDIRECTION_APPEND || args_head->type == HEREDOC ||
			args_head->type == PIPE)
			return count;
		count++;
		args_head = args_head->next;
	}
	return count;
}

// fork and execve
void redirect_execve_args(t_arg *args_head)
{
	t_arg *tmp;
	pid_t pid;
	char *path;
	char **args;
	int i;
	int argc;

	tmp = args_head;
	argc = redirect_count_arguments(tmp);
	args = (char **)ft_gc_malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (tmp && i < argc)
	{
		if (tmp->type == WORD || tmp->type == DOUBLE_QUOTED_STRING ||
			tmp->type == SINGLE_QUOTED_STRING)
		{
			args[i] = ft_shell_strdup(remove_quotes(tmp->arg));
			i++;
		}
		tmp = tmp->next;
	}
	args[i] = NULL;

	path = get_path(args[0]);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)
	{
		// In child process
		if (is_built_in(args[0]))
		{
			exec_built_ins(args_head);
			exit(0);
		}
		else if (execve(path, args, environ) == -1) // Use 'environ' to pass environment variables
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// In parent process
		waitpid(pid, NULL, 0);
	}
}
