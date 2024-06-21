/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:16:01 by afoth             #+#    #+#             */
/*   Updated: 2024/06/21 15:39:50 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	memory_error(char *message)
{
	ft_printf("Memory error: %s\n", message);
	perror(message);
	ft_gc_free();
	exit(EXIT_FAILURE);
}
