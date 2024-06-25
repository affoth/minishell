/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/25 17:14:29 by mokutucu         ###   ########.fr       */
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
	return (line);
}
//main minishell
int	main()
{
	char	*input;
	char	*expanded;
	t_arg	*args_head; // Declare args_head here

	signal_init();
	while (1)
	{
		input = get_input();
		expanded = expand_string(input);
		if (!expanded)
			continue;
		add_history(expanded);
		args_head = tokenizer(expanded);
		if (find_redirections_and_pipes(args_head))
			handle_redirections_and_pipes(args_head);
		else
		{
			exec_built_ins(args_head);
			execve_args(args_head);
		}
		free(input);
		free(expanded);
	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



