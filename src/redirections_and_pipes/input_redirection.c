/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:28:22 by afoth             #+#    #+#             */
/*   Updated: 2024/06/20 22:38:28 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	handle_redirections_and_pipes(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	if (head == NULL)
		return ;
	while (head != NULL)
	{
		if (head->type == REDIRECTION_IN)
		{
			input_redirection(head, tmp);
		}
		else if (head->type == REDIRECTION_OUT)
		{
			output_redirection(head, tmp);
		}
		// else if (head->type == REDIRECTION_APPEND)
		// {

		// }
		// else if (head->type == HEREDOC)
		// {

		// }
		// else if (head->type == PIPE)
		// {

		// }
		head = head->next;
	}
}

void	input_redirection(t_arg *head, t_arg *tmp)
{
	int	fd;
	int	dup2_check;

	check_file_readable(head->next->arg);
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
	// close(fd);
}

//normal error Message: bash: test.txt: Permission denied
//bash: create.txt: No such file or directory
void	check_file_readable(const char *filepath)
{
	if (access(filepath, F_OK) != 0)
	{
		perror("File does not exist.");
		ft_gc_free();
		exit(EXIT_FAILURE);
		//return (-1);
	}
	else if (access(filepath, R_OK) != 0)
	{
		perror("File is not readable.");
		ft_gc_free();
		exit(EXIT_FAILURE);
		//return (-1);
	}
}

