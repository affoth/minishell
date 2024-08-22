/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/22 15:54:28 by mokutucu         ###   ########.fr       */
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

		execute_with_pipes(&gc, args_head); // Call the function to handle pipes

		free(input);
		free(expanded);
	}
	ft_gc_free(&gc);
	rl_clear_history();
	return (0);
}



