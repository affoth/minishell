/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:05:32 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/03 19:58:42 by mokutucu         ###   ########.fr       */
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
	ft_printf("%s\n", current_dir);
	free(current_dir);
}
