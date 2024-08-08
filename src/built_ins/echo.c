/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:28:26 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/08 17:09:23 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Check if the initial flags are -n
bool check_initial_n_flag(t_arg **args_head)
{
	t_arg *tmp;
	bool suppress_newline;

	suppress_newline = false;
	tmp = *args_head;
	while (tmp && ft_strcmp(tmp->arg, "-n") == 0)
	{
		suppress_newline = true;
		tmp = tmp->next;
	}
	*args_head = tmp;  // Update args_head to skip the -n flags
	return suppress_newline;
}

// Print the arguments, preserving spacing
void print_echo_arguments(t_arg *args_head)
{
	t_arg *tmp = args_head;
	bool first_arg = true;

	while (tmp)
	{
		if (!first_arg)
			printf(" ");
		printf("%s", tmp->arg);
		first_arg = false;
		tmp = tmp->next;
	}
}

// echo -n built-in
void	built_in_echo(t_arg *args_head)
{
	bool suppress_newline;
	// Skip the command name itself
	if (args_head)
		args_head = args_head->next;

	// Skip the -n flags
	suppress_newline = false;
	while (args_head && ft_strcmp(args_head->arg, "-n") == 0)
	{
		suppress_newline = true;
		args_head = args_head->next;
	}

	print_echo_arguments(args_head);
	if (!suppress_newline)
		ft_printf("\n");
}

