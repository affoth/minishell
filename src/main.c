/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/27 22:35:47 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//getting input and making sure that no input gives new prompt or exits (ctrl+d)
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

	signal_init();
	while (1)
	{
		input = get_input();
		expanded = expand_string(input);
		if (!expanded)
			continue;
		args_head = tokenizer(expanded);
		if (!args_head)
			continue;

		if (find_redirections_and_pipes(args_head) > 1)
			multiple_redirections(args_head);
		else if (find_redirections_and_pipes(args_head) == 1)
			handle_redirection_or_pipe(args_head);
		else if (is_built_in(args_head))
			exec_built_ins(args_head);
		else
			execve_args(args_head);
		free(input);
		free(expanded);
	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



