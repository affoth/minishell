/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/22 17:10:48 by mokutucu         ###   ########.fr       */
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
		if (!ft_input_check(line))
			parser(line);
		add_history(line);
		free(line);
		rl_clear_history();
	}
	return (0);
}

// input check
// DEL erstmal um zu sehen ob die anfuehrungszeichen geschlossen sind
// DEL needs testing
void	ft_input_check(char *line)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (line != '\0')
	{
		if (line[i] = "'")//39
			single_quotes++;
		if (line[i] = 34)
			double_quotes++;
		i++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		//DEL do stuff
		perror("Quotations not closed");
		exit (1);
	}
}


