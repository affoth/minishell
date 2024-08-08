/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:31:38 by afoth             #+#    #+#             */
/*   Updated: 2024/08/08 17:00:20 by afoth            ###   ########.fr       */
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
void	output_redirection(t_gc *gc, t_arg *first_arg, t_arg *second_arg, int fd_input)
{
	int	fd;
	int	stdout_save;
	pid_t	pid;
	printf("second_arg->arg: %s\n", second_arg->arg);
	//check_file_writable(head->next->arg);
	fd = open(second_arg->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open");
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
		perror("dup");


	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(fd);
		return;
	}
	if (pid > 0)
	{
		//printf("\noutchild\n");
		//printf("pidout: %d\n", pid);
	}
	if (pid == 0)
	{
		if (fd_input != -1 && fd_input != -2)
		{
			if (dup2(fd_input, STDIN_FILENO) == -1)
			{
				perror("dup2");
			}
			close(fd_input);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
		redirect_execve_args(gc, first_arg);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, NULL, 0);
	if (dup2(stdout_save, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdout_save);
	close(fd);
}

//this is the original function
void	simple_output_redirection(t_gc *gc, t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int	stdout_save;

	//check_file_writable(head->next->arg);
	fd = open(head->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open");
	stdout_save = dup(STDOUT_FILENO);
	if (stdout_save == -1)
		perror("dup");
	dup2_check = dup2(fd, STDOUT_FILENO);
	if (dup2_check == -1)
		perror("dup2");
	redirect_execve_args(gc, tmp);
	dup2(stdout_save, STDOUT_FILENO);
	if (dup2(stdout_save, STDOUT_FILENO) == -1)
		perror("dup2");
	close(stdout_save);
	close(fd);
}
