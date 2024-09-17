/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 23:02:49 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**init_env(char **envp, t_gc *gc)
{
	char	**env_copy;
	int		env_len;
	int		i;

	env_len = ft_env_len(envp);
	env_copy = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
	if (!env_copy)
	{
		perror("Failed to allocate memory for environment copy");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < env_len)
	{
		env_copy[i] = ft_shell_strdup(gc, envp[i]);
		i++;
	}
	env_copy[env_len] = NULL;

	return (env_copy);
}

char	*get_input()
{
	char *line;

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

void	init_shell(t_shell *shell, char **envp)
{
	ft_gc_init(&shell->gc, shell);
	shell->env = init_env(envp, &shell->gc);
	shell->cmds_head = NULL;
	shell->exit_status = 0;
	setup_signals();
}


void	print_cmd_args(char **args, const char *label)
{
	int	i;

	if (args)
	{
		printf("%s:\n", label);
		i = 0;
		while (args[i])
		{
			printf("  Arg %d: %s\n", i, args[i]);
			i++;
		}
	}
	else
	{
		printf("%s: NULL\n", label);
	}
}

void	print_commands(t_command *cmds_head)
{
	t_command	*cmd;

	cmd = cmds_head;


	while (cmd)
	{
		printf("Command:\n");
		printf("  stdin_fd: %d\n", cmd->stdin_fd);
		printf("  stdout_fd: %d\n", cmd->stdout_fd);
		printf("  Append mode: %s\n", cmd->append_mode ? "true" : "false");
		printf("  Command name: %s\n", cmd->cmd_name);
		print_cmd_args(cmd->flags, "Flags");
		print_cmd_args(cmd->args, "Args");
		cmd = cmd->next;
		printf("-----\n");
	}
}

// Function to check if piping is needed
int	needs_piping(t_command *cmds_head)
{
	t_command *current_cmd;

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

// Main loop for shell
//print_commands(shell->cmds_head);
void	execute_shell(t_shell *shell)
{
	char	*input;
	char	*expanded_vars;
	t_arg	*args_head;
	int		pipe_count;

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
			continue ;
		}
		pipe_count = count_pipes_argstruct(args_head);
		shell->cmds_head = create_and_populate_commands
			(shell, &shell->gc, args_head, pipe_count);
		if (needs_piping(shell->cmds_head))
			shell->exit_status = execute_commands_with_pipes
				(shell, shell->cmds_head);
		else
			shell->exit_status = execute_command_without_pipes
				(shell, shell->cmds_head);
		free(input);
	}
}

 // Main shell execution loop
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell	shell;

	init_shell(&shell, envp);
	execute_shell(&shell);
	ft_gc_free(&shell.gc);
	return (0);
}


