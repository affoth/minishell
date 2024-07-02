/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:00:16 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/02 18:26:33 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_signal g_signal = {0, 0, 0, 0};

void sigint_handler_parent(int num)
{
    (void)num;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    g_signal.sigint_received = 1;
}

void sigint_handler_child(int num)
{
    (void)num;
	write(STDOUT_FILENO, "\n", 1);
	_exit(1);
    g_signal.sigint_received = 1;
}

void sigquit_handler(int num)
{
    (void)num;
    g_signal.sigquit_received = 1;
	write(1, "Quit: ", 6);
    write(1, "\n", 1);
}

void set_signals_parent(void)
{
    signal(SIGINT, sigint_handler_parent);
    signal(SIGQUIT, SIG_IGN);
}

void set_signals_child(void)
{
    signal(SIGINT, sigint_handler_child);
    signal(SIGQUIT, SIG_IGN);
}
