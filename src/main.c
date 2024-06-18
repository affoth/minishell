/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/18 18:44:22 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//main minishell
int	main()
{
	char	*line;
	char	*expanded;
	t_arg	*args_head; // Declare args_head here

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		add_history(line);

		expanded = expand_string(line);
		ft_printf("Expanded: %s\n", expanded);
		args_head = tokenizer(expanded);

		exec_built_ins(args_head);
		execve_args(args_head);
		free(line);
	}
	ft_gc_free();
	rl_clear_history();
	return (0);
}



