/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:09:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 20:31:11 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Check list for syntax errors and return 1 if there is an error
int	syntax_checker(t_arg *head)
{
	if (!head)
	{
		return (1);
	}
	if (pipe_syntax(head))
	{
		return (1);
	}
	if (redirection_syntax(head))
	{
		return (1);
	}
	return (0);
}
