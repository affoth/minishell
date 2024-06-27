/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:28:22 by afoth             #+#    #+#             */
/*   Updated: 2024/06/27 15:42:54 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"




void	input_redirection(t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;

	if (!check_file_readable(head->next->arg))
	{
		fd = open(head->next->arg, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			ft_gc_free();
			exit(EXIT_FAILURE);
		}
		int stdin_save = dup(STDIN_FILENO);
		dup2_check = dup2(fd, STDIN_FILENO);
		if (dup2_check == -1)
		{
			perror("dup2");
			ft_gc_free();
			exit(EXIT_FAILURE);
		}
		printf("\nInput redirection: %s\n", head->next->arg);
		printf("fd: %d\n", fd);
		printf("STDIN_FILENO: %d\n\n", STDIN_FILENO);
		//Execute command
		redirect_execve_args(tmp);
		dup2(stdin_save, STDIN_FILENO);
		close(stdin_save);
		close(fd);
	}
}

//normal error Message: bash: test.txt: Permission denied
//bash: create.txt: No such file or directory
int	check_file_readable(const char *filepath)
{
	printf("filepath: %s\n", filepath);
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

