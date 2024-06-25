/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:33:45 by afoth             #+#    #+#             */
/*   Updated: 2024/06/25 23:04:23 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	find_redirections_and_pipes(t_arg *head)
{
	while (head != NULL)
	{
		if (head->type == REDIRECTION_IN || head->type == REDIRECTION_OUT || head->type == REDIRECTION_APPEND || head->type == HEREDOC || head->type == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
}

int	count_pipes(t_arg *head)
{
	int	count;

	count = 0;
	while (head)
	{
		if (head->type == PIPE)
			count++;
		head = head->next;
	}
	return (count);
}
// void check_file_access(const char *filepath) {
//     if (access(filepath, F_OK) == 0) {
//         printf("File exists.\n");

//         if (access(filepath, R_OK) == 0) {
//             printf("File is readable.\n");
//         } else {
//             printf("File is not readable.\n");
//         }

//         if (access(filepath, X_OK) == 0) {
//             printf("File is executable.\n");
//         } else {
//             printf("File is not executable.\n");
//         }
//     } else {
//         printf("File does not exist.\n");
//     }
// }

// void	check_file_readable(const char *filepath)
// {
// 	if (access(filepath, F_OK) != 0)
// 	{
// 		perror("File does not exist.");
// 		ft_gc_free();
// 		exit(EXIT_FAILURE);
// 		//return (-1);
// 	}
// 	else if (access(filepath, R_OK) != 0)
// 	{
// 		perror("File is not readable.");
// 		ft_gc_free();
// 		exit(EXIT_FAILURE);
// 		//return (-1);
// 	}
// }
//on fail do i exit or return -1? Do I use perror or printf?



