/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/25 17:08:15 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//getting input and making sure that no input gives new prompt or exits (ctrl+d)
char	*get_input()
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
		return NULL;
	}
	free(line);
	return (line);
}
//main minishell
int	main()
{
	char	*input;
	t_arg	*args_head; // Declare args_head here

	signal_init();
	while (1)
	{
		input = get_input();
		input = expand_string(input);
		if (!input)
			continue;
		add_history(input);
		args_head = tokenizer(input);
		if (find_redirections_and_pipes(args_head))
			handle_redirections_and_pipes(args_head);
		else
		{
			exec_built_ins(args_head);
			execve_args(args_head);
		}
		free(input);

	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



