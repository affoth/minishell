/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/30 16:54:12 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//main minishell
int	main()
{
	char	*line;
	t_arg	*args_head; // Declare args_head here

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		add_history(line);

		args_head = tokenizer(line);
		if (args_head == NULL) 
			continue;
		exec_built_ins(args_head);
		// free garbage collector);
		ft_gc_free();
		free(line);
	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



