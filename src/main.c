/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/13 15:26:25 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//main minishell

int	main()
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		add_history(line);
		free(line);
	}
	return (0);
}
