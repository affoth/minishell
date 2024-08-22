/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:31:38 by afoth             #+#    #+#             */
/*   Updated: 2024/08/22 15:41:33 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


//is this still needed because of open flags?
void	check_file_writable(const char *filepath)
{
	if (access(filepath, W_OK) != 0)
	{
		perror("File is not writable.");

		//return (-1);
	}
}
/*O_WRONLY | O_CREAT | O_TRUNC,:
These are the flags for the open call.
 O_WRONLY means the file is opened for writing only.
 O_CREAT means the file will be created if it doesn't already exist.
 O_TRUNC means the file will be truncated to zero length if it already exists and is a regular file.
0644): This is the file mode,
which is only relevant because O_CREAT is one of the flags.
0644 is an octal number that indicates the permissions of the file if it's created.
In this case, 0644 means the owner of the file will have read
and write permissions, and everyone else will have read permissions.*/

void	simple_output_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int	stdout_save;

	fd = open(head->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}

	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
	{
		perror("dup");
		close(fd);
		return;
	}

	dup2_check = dup2(fd, STDOUT_FILENO);
	if (dup2_check == -1)
	{
		perror("dup2");
		close(stdout_save);
		close(fd);
		return;
	}

	// Execute the command
	redirect_execve_args(gc, tmp);

	// Restore original stdout
	if (dup2(stdout_save, STDOUT_FILENO) == -1)
		perror("dup2");

	close(stdout_save);
	close(fd);
}
