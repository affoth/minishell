/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:05:32 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/05 18:37:28 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void built_in_pwd(void)
{
	char *current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		ft_printf("Error getting current directory\n");
		return;
	}
	write(STDOUT_FILENO, current_dir, ft_strlen(current_dir));
	free(current_dir);
}
