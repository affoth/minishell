/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:33:45 by afoth             #+#    #+#             */
/*   Updated: 2024/08/09 16:28:33 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirection_or_pipe(t_gc *gc, t_arg *tmp_position)
{
	t_arg	*head_of_struct;

	head_of_struct = tmp_position;
	if (tmp_position == NULL)
		return ;
	while (tmp_position != NULL)
	{
		if (tmp_position->type == REDIRECTION_IN)
		{
			simple_input_redirection(gc, tmp_position, head_of_struct);
		}
		else if (tmp_position->type == REDIRECTION_OUT)
		{
			simple_output_redirection(gc, tmp_position, head_of_struct);
		}
		else if (tmp_position->type == REDIRECTION_APPEND)
		{
			simple_append_redirection(gc, tmp_position, head_of_struct);
		}
		else if (tmp_position->type == HEREDOC)
		{
			// Get the delimiter which is the next argument
            if (tmp_position->next && (tmp_position->next->type == WORD ||
                                        tmp_position->next->type == DOUBLE_QUOTED_STRING ||
                                        tmp_position->next->type == SINGLE_QUOTED_STRING))
            {
                heredoc(tmp_position->next->arg);
            }
			//need an else statement?
		}
		else if (tmp_position->type == PIPE)
		{
			simple_pipe_redirection(gc, tmp_position, head_of_struct);
		}
		tmp_position = tmp_position->next;
	}
}

int	find_redirections_and_pipes(t_arg *head)
{
	int	count;

	count = 0;
	while (head != NULL)
	{
		if (head->type == REDIRECTION_IN || head->type == REDIRECTION_OUT || head->type == REDIRECTION_APPEND || head->type == HEREDOC || head->type == PIPE)
			count++;
		head = head->next;
	}
	return (count);
}

t_arg	*search_for_next_redirection(t_arg *tmp)
{
	while (tmp)
	{
		if (tmp->type == REDIRECTION_IN || tmp->type == REDIRECTION_OUT || tmp->type == REDIRECTION_APPEND || tmp->type == HEREDOC || tmp->type == PIPE)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}


/* int	count_pipes(t_arg *head)
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
} */
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



