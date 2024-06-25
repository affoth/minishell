/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/25 14:52:07 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//main minishell
int	main()
{
	char	*line;
	char	*expanded;
	t_arg	*args_head; // Declare args_head here

	signal_init();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		else
		{
			add_history(line);
			expanded = expand_string(line);

			args_head = tokenizer(expanded);
			if (find_redirections_and_pipes(args_head))
				handle_redirections_and_pipes(args_head);
			else
			{
				exec_built_ins(args_head);
				execve_args(args_head);
			}
			free(line);
		}
	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



