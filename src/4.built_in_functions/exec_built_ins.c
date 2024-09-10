/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_ins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/04 19:48:32 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to check if a command is a built-in
int is_built_in(char *arg)
{
    if (ft_strcmp(arg, "cd") == 0)
        return 1;
    if (ft_strcmp(arg, "echo") == 0)
        return 1;
    if (ft_strcmp(arg, "pwd") == 0)
        return 1;
    if (ft_strcmp(arg, "env") == 0)
        return 1;
    if (ft_strcmp(arg, "export") == 0)
        return 1;
    if (ft_strcmp(arg, "unset") == 0)
        return 1;
    if (ft_strcmp(arg, "exit") == 0)
        return 1;
    return 0;
}

// Function to execute built-in commands
void exec_built_ins(t_shell *shell)
{
    t_command *cmd = shell->cmds_head;
    char *cmd_name;

    if (cmd == NULL)
        return;

    // Extract command name and arguments
    cmd_name = cmd->cmd_name;
    if (cmd_name == NULL)
        return;

    if (ft_strcmp(cmd_name, "cd") == 0)
        built_in_cd(shell);
    else if (ft_strcmp(cmd_name, "echo") == 0)
        built_in_echo(shell);  // Pass args directly if echo uses them
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        built_in_pwd();
    else if (ft_strcmp(cmd_name, "env") == 0)
        built_in_env(shell);
    else if (ft_strcmp(cmd_name, "export") == 0)
        built_in_export(shell);  // Pass args directly if export uses them
    else if (ft_strcmp(cmd_name, "unset") == 0)
        built_in_unset(shell);  // Pass args directly if unset uses them
    else if (ft_strcmp(cmd_name, "exit") == 0)
        built_in_exit(shell);  // Pass args directly if exit uses them

}
