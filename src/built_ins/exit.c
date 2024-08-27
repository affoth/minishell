/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:28:26 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/28 00:06:47 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if a string is a number
bool is_number(const char *str)
{
    if (*str == '-' || *str == '+')
        str++;
    while (*str)
    {
        if (!ft_isdigit(*str))
            return false;
        str++;
    }
    return true;
}

// Exit built-in command
void built_in_exit(t_arg *args_head)
{
    int exit_code = 0;

    if (args_head->next)
    {
        if (!is_number(args_head->next->arg))
        {
            ft_printf("exit: %s: numeric argument required\n", args_head->next->arg);
            exit_code = 255;
        }
        else if (args_head->next->next)
        {
            ft_printf("exit: too many arguments\n");
            return;
        }
        else
        {
            exit_code = ft_atoi(args_head->next->arg);
        }
    }
    ft_printf("exit\n");
    exit(exit_code);
}