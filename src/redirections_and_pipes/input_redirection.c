/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:28:22 by afoth             #+#    #+#             */
/*   Updated: 2024/08/22 15:46:30 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int check_file_readable(const char *filepath)
{
    if (access(filepath, F_OK) != 0)
    {
        printf("File does not exist.\n");
        return (1);
    }
    else if (access(filepath, R_OK) != 0)
    {
        printf("File is not readable.\n");
        return (1);
    }
    return (0);
}

void	simple_input_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int stdin_save;

	if (check_file_readable(head->next->arg))
		return;

	fd = open(head->next->arg, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return;
	}

	stdin_save = dup(STDIN_FILENO);
	if (stdin_save == -1)
	{
		perror("dup");
		close(fd);
		return;
	}

	dup2_check = dup2(fd, STDIN_FILENO);
	if (dup2_check == -1)
	{
		perror("dup2");
		close(stdin_save);
		close(fd);
		return;
	}

	// Execute the command
	redirect_execve_args(gc, tmp);

	// Restore original stdin
	if (dup2(stdin_save, STDIN_FILENO) == -1)
		perror("dup2");

	close(stdin_save);
	close(fd);
}
