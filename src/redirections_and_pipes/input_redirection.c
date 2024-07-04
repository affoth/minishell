/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:28:22 by afoth             #+#    #+#             */
/*   Updated: 2024/07/04 17:07:03 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//ORINGINAL FUNCTION
void	simple_input_redirection(t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	int stdin_save;

	if (!check_file_readable(head->next->arg))
	{
		fd = open(head->next->arg, O_RDONLY);
		if (fd == -1)
			perror("open");
		stdin_save = dup(STDIN_FILENO);
		if (stdin_save == -1)
			perror("dup");
		dup2_check = dup2(fd, STDIN_FILENO);
		if (dup2_check == -1)
			perror("dup2");
		redirect_execve_args(tmp);
		dup2(stdin_save, STDIN_FILENO);
		close(stdin_save);
		close(fd);
	}
}


int	input_redirection(t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;
	t_arg	*next_redirection;
	//int stdin_save;

	if (!check_file_readable(head->next->arg))
	{
		fd = open(head->next->arg, O_RDONLY);
		if (fd == -1)
			perror("open");
		next_redirection = search_for_next_redirection(tmp);
		dup2_check = dup2(fd, STDIN_FILENO);
		if (dup2_check == -1)
			perror("dup2");
		if (next_redirection->type == PIPE || next_redirection->type == REDIRECTION_OUT || next_redirection->type == REDIRECTION_APPEND)
			return (fd);
		//stdin_save = dup(STDIN_FILENO);
		//if (stdin_save == -1)
			//perror("dup");

		redirect_execve_args(tmp);
		//dup2(stdin_save, STDIN_FILENO);
		//close(stdin_save);
		//close(fd);
		return (fd);
	}
	return (-1);
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

