/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:16:07 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/19 19:40:08 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**init_env(char **envp, t_gc *gc)
{
	char	**env_copy;
	int		env_len;
	int		i;

	env_len = ft_env_len(envp);
	env_copy = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
	if (!env_copy)
	{
		perror("Failed to allocate memory for environment copy");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < env_len)
	{
		env_copy[i] = ft_shell_strdup(gc, envp[i]);
		i++;
	}
	env_copy[env_len] = NULL;
	return (env_copy);
}

void	init_shell(t_shell *shell, char **envp)
{
	ft_gc_init(&shell->gc, shell);
	shell->env = init_env(envp, &shell->gc);
	shell->cmds_head = NULL;
	shell->exit_status = 0;
}
