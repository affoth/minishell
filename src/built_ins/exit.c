/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 17:20:25 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/08 17:07:38 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_isnumber(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
void	built_in_exit(t_arg *args_head)
{
	t_arg	*tmp;
	int		ex_code;

	tmp = args_head->next;
	if (!tmp)
		exit(0);

	if (tmp->next)
	{
		printf("exit: too many arguments\n");
		return;
	}

	if (!ft_isnumber(tmp->arg) || ft_atoi(tmp->arg) < 0 || ft_atoi(tmp->arg) > 255)
	{
		printf("exit: %s: numeric required from 0 to 255\n", tmp->arg);
		exit(255);
	}
	ex_code = ft_atoi(tmp->arg);
	printf("exit status: %d\n", ex_code);
	exit(ex_code);
}
