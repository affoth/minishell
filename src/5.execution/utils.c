/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:49 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/14 04:05:59 by mokutucu         ###   ########.fr       */
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

// Function to count the number of pipes needed based on the command structure
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

// Set up redirections for a given command in the pipeline
void setup_redirections(int cmd_index, int num_pipes, int *pipe_descriptors)
{
    if (cmd_index > 0)
    {
        // Redirect stdin from the previous pipe
        if (dup2(pipe_descriptors[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
    }

    if (cmd_index < num_pipes)
    {
        // Redirect stdout to the next pipe
        if (dup2(pipe_descriptors[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
        {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
    }
}

// Create pipes for the specified number of pipes
void create_pipes(int num_pipes, int *pipe_descriptors)
{
    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipe_descriptors + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
}

// Close all pipe descriptors
void close_pipes(int num_pipes, int *pipe_descriptors)
{
    for (int i = 0; i < 2 * num_pipes; i++)
    {
        close(pipe_descriptors[i]);
    }
}
