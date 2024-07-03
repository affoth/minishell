/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:10:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/01 15:49:51 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void heredoc(t_arg *head, t_arg *tmp)
{
	head = head->next;
	tmp = head;

	while (tmp->type != HEREDOC)
	{
		printf("heredoc> ");
		tmp->arg = readline("");
		if (tmp->arg == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		tmp = tmp->next;
	}
}
