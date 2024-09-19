/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 19:33:39 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (*line == '\0')
	{
		free(line);
		return (NULL);
	}
	add_history(line);
	return (line);
}

// Function to check if piping is needed
int	needs_piping(t_command *cmds_head)
{
	t_command	*current_cmd;

	current_cmd = cmds_head;
	while (current_cmd)
	{
		if (current_cmd->next)
		{
			return (1);
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

void	execute_shell_is_piping_needed(t_shell *shell)
{
	if (needs_piping(shell->cmds_head))
		shell->exit_status = execute_commands_with_pipes
			(shell, shell->cmds_head);
	else
		shell->exit_status = execute_command_without_pipes
			(shell, shell->cmds_head);
}

// Main loop for shell
//print_commands(shell->cmds_head);
void	execute_shell(t_shell *shell)
{
	char	*input;
	char	*expanded_vars;
	t_arg	*args_head;

	while (1)
	{
		input = get_input();
		if (!input)
		{
			shell->exit_status = 0;
			free(input);
			continue ;
		}
		expanded_vars = expand_string(shell, input, shell->exit_status);
		args_head = tokenizer(shell, expanded_vars);
		if (syntax_checker(args_head) == 1)
		{
			shell->exit_status = 2;
			free(input);
			continue ;
		}
		shell->cmds_head = create_and_populate_commands
			(shell, &shell->gc, args_head);
		execute_shell_is_piping_needed(shell);
		free(input);
	}
}

// Main shell execution loop
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	execute_shell(&shell);
	ft_gc_free(&shell.gc);
	return (0);
}
