/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:28:22 by afoth             #+#    #+#             */
/*   Updated: 2024/06/18 18:26:24 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	handle_redirections_and_pipes(t_arg *head)
{
	if (head == NULL)
		return ;
	while (head != NULL)
	{
		if (head->type == REDIRECTION_IN)
		{
			input_redirection(head);
		}
		// else if (head->type == REDIRECTION_OUT)
		// {

		// }
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

void	input_redirection(t_arg *head)
{
	int	fd;

	check_file_readable(head->next->arg);
	fd = open(head->next->arg, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		ft_gc_free();
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_gc_free();
		exit(EXIT_FAILURE);
	}
	// close(fd);
}

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


