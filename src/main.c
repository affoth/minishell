/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/04 16:55:00 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// getting input and making sure that no input gives new prompt or exits (ctrl+d)

char	*get_input()
{
	char	*line;

	line = readline("minishell$ ");
	add_history(line);
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (*line == '\0')
	{
		free(line);
		return NULL;
	}
	return (line);
}
//main minishell
int	main()
{
	char	*input;
	char	*expanded;
	t_arg	*args_head; // Declare args_head here
	t_gc	gc;

	// Initialize the garbage collector
	gc.head = NULL;

	set_signals_parent();
	while (1)
	{
		input = get_input();
		expanded = expand_string(&gc, input);
		if (!expanded)
			continue;
		args_head = tokenizer(&gc, expanded);
		if (!args_head)
			continue;

		if (find_redirections_and_pipes(args_head) > 1)
			multiple_redirections(&gc, args_head);
		else if (find_redirections_and_pipes(args_head) == 1)
			handle_redirection_or_pipe(&gc, args_head);
		else if (is_built_in(args_head->arg))
			exec_built_ins(&gc, args_head);
		else
			execve_args(&gc, args_head);
		free(input);
		free(expanded);
	}
	ft_gc_free(&gc);
	rl_clear_history();
	return (0);
}



