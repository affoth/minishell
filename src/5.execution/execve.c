/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:16:51 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/05 16:00:12 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void execute_command(t_shell *shell, t_command *cmd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Prepare command arguments
        int flags_count = 0;
        while (cmd->flags && cmd->flags[flags_count])
            flags_count++;

        int args_count = 0;
        while (cmd->args && cmd->args[args_count])
            args_count++;

        char **args = (char **)ft_gc_malloc(&shell->gc, sizeof(char *) * (flags_count + args_count + 2));
        if (!args)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (i < flags_count)
        {
            args[i + 1] = ft_shell_strdup(&shell->gc, cmd->flags[i]);
            if (!args[i + 1])
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        args[0] = ft_shell_strdup(&shell->gc, cmd->cmd_name);
        if (!args[0])
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }

        int j = 0;
        while (j < args_count)
        {
            args[flags_count + 1 + j] = remove_quotes(&shell->gc,strdup(cmd->args[j]));
            if (!args[flags_count + 1 + j])
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            j++;
        }
        args[flags_count + args_count + 1] = NULL;

        // Resolve the command path
        char *path = get_path(&shell->gc, args[0]);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", args[0]);
            exit(EXIT_FAILURE);
        }

        execve(path, args, shell->env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Parent process should wait for the child process to complete
    while (wait(NULL) > 0);
}
