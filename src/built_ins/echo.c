/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:28:26 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/03 18:58:44 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Check if -n flag is present in arguments
bool	has_suppress_newline_flag(t_arg *args_head)
{
	t_arg *tmp = args_head;
	while (tmp)
	{
		if (ft_strcmp(tmp->arg, "-n") == 0)
			return true;
		tmp = tmp->next;
	}
	return false;
}

// Print the arguments, excluding -n if present
void	print_echo_arguments(t_arg *args_head)
{
	t_arg *tmp = args_head;
	while (tmp)
	{
		if (ft_strcmp(tmp->arg, "-n") != 0)
		{
			if (tmp->next)
				ft_printf(" ");
		}
		tmp = tmp->next;
	}
}

// echo -n built in
void	built_in_echo(t_arg *args_head)
{
	bool suppress_newline;

	suppress_newline = has_suppress_newline_flag(args_head);
	print_echo_arguments(args_head);
	if (!suppress_newline)
		ft_printf("\n");
}

