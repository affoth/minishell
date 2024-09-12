/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:49 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 20:01:34 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to count arguments
int count_arguments(char **args)
{
    int count = 0;

    while (args[count])
        count++;
    return count;
}

int count_pipes_cmdstruct(t_command *cmds_head)
{
    int count = 0;
    while (cmds_head && cmds_head->next)
    {
        count++;
        cmds_head = cmds_head->next;
    }
    return count;
}

void setup_redirections(int cmd_index, int num_pipes, int *pipe_descriptors)
{
    if (cmd_index > 0)
    {
        if (dup2(pipe_descriptors[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
    }

    if (cmd_index < num_pipes)
    {
        if (dup2(pipe_descriptors[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
    }
}

void create_pipes(int num_pipes, int *pipe_descriptors)
{
    int i = 0;
    while (i < num_pipes)
    {
        if (pipe(pipe_descriptors + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

void close_pipes(int num_pipes, int *pipe_descriptors)
{
    int i = 0;
    while (i < 2 * num_pipes)
    {
        close(pipe_descriptors[i]);
        i++;
    }
}