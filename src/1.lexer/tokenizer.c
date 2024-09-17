/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:09:46 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 20:34:18 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Mapping struct for token types that are required by subject pdf
const	Token typeMap[] =
{
	{"|", PIPE},
	{"\"", DOUBLE_QUOTED_STRING},
	{"'", SINGLE_QUOTED_STRING},
	{"$", ENV_VARIABLE},
	{"<<", HEREDOC},
	{">>", REDIRECTION_APPEND},
	{"<", REDIRECTION_IN},
	{">", REDIRECTION_OUT},
	{NULL, WORD}
};

TokenType	get_token_type(const char *arg)
{
	int	i;

	i = 0;
	if (strcmp(arg, "<<") == 0)
		return (HEREDOC);
	if (strcmp(arg, ">>") == 0)
		return (REDIRECTION_APPEND);
	if (strcmp(arg, "<") == 0)
		return (REDIRECTION_IN);
	if (strcmp(arg, ">") == 0)
		return (REDIRECTION_OUT);


	while (typeMap[i].arg != NULL)
	{
		if (strcmp(arg, typeMap[i].arg) == 0)
			return (typeMap[i].type);
		i++;
	}
	if (arg[0] == '-' && arg[1] != '\0' && isalpha((unsigned char)arg[1]))
		return (FLAGS);
	return (WORD);
}

t_arg	*create_arg_node(t_gc *gc, const char *arg)
{
	t_arg	*node;

	node = ft_gc_malloc(gc, sizeof(t_arg));
	if (node == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	node->arg = strdup(arg);
	if (node->arg == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	node->type = get_token_type(arg);
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	add_arg_to_list(t_gc *gc, t_arg **head, const char *arg)
{
	t_arg	*new_node;
	t_arg	*current_node;

	new_node = create_arg_node(gc, arg);
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	current_node = *head;
	while (current_node->next != NULL)
	{
		current_node = current_node->next;
	}
	current_node->next = new_node;
	new_node->prev = current_node;
}

void	print_tokens(t_arg *head)
{
	t_arg	*current;

	current = head;
	while (current != NULL)
	{
		printf("Token: %s, Type: %d\n", current->arg, current->type);
		current = current->next;
	}
}

// Split the input string into tokens
//DEBUG print_tokens(args_head);

t_arg	*tokenizer(t_shell *shell, char *input)
{
	size_t	i;
	char	**tokens;
	t_arg	*args_head;

	i = 0;
	if (!input)
	{
		write(STDERR_FILENO, "Error: Null input line\n", 23);
		shell->exit_status = 1;
		return (NULL);
	}
	tokens = ft_split_redirections(&shell->gc, input);
	if (!tokens)
	{
		write(STDERR_FILENO, "Error: Memory allocation failed\n", 32);
		shell->exit_status = 1;
		return (NULL);
	}
	args_head = NULL;
	while (tokens[i] != NULL)
	{
		add_arg_to_list(&shell->gc, &args_head, tokens[i]);
		i++;
	}
	return (args_head);
}
