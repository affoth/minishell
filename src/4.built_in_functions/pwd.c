/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:05:32 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/11 18:44:22 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Return codes
#define PWD_SUCCESS 0
#define PWD_ERROR 1

int built_in_pwd(void)
{
    char *current_dir;

    current_dir = getcwd(NULL, 0);
    if (!current_dir)
    {
        perror("getcwd");
        return PWD_ERROR; // Return error code if getcwd fails
    }

    write(STDOUT_FILENO, current_dir, ft_strlen(current_dir));
    write(STDOUT_FILENO, "\n", 1);
    free(current_dir);

    return PWD_SUCCESS; // Return success code
}
