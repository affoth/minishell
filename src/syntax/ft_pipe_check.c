/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:09:04 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/27 20:23:00 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//check if pipe syntax is correctly used in input
int	ft_pipe_check(char *line)
{
    int	i;

    i = 0;
    while (line[i])
    {
        if (line[i] == '|')
        {
            if (line[i + 1] == '|')
            {
                printf("syntax error near unexpected token `||'\n");
                return (1);
            }
            if (line[i - 1] == '|')
            {
                printf("syntax error near unexpected token `|'\n");
                return (1);
            }
        }
        i++;
    }
    return (0);
}