/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:08:45 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/29 15:11:55 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

// typedef enum TokenType
// {
//     WORD,                // Generic word (command or argument)
//     REDIRECTION_OUT,     // >
//     REDIRECTION_IN,      // <
//     REDIRECTION_APPEND,  // >>
//     HEREDOC,             // <<
//     PIPE,                // |
//     AND,                 // &&
//     OR,                  // ||
//     OPEN_PAREN,          // (
//     CLOSE_PAREN,         // )
//     DOUBLE_QUOTED_STRING, // Double-quoted string
//     SINGLE_QUOTED_STRING, // Single-quoted string
//     ENV_VARIABLE,        // Environment variable (like $HOME)
//     END                  // End of input
// } TokenType;

// // Argument struct
// typedef struct s_arg
// {
//     char *arg;
//     enum TokenType type;
//     struct s_arg *prev;
//     struct s_arg *next;
// } t_arg;

// // Token struct
// typedef struct {
//     char *arg;
//     TokenType type;
// } Token;

// Mapping struct for token types that are required by subject pdf
const Token typeMap[] =
{
	{"|", PIPE},
	{"&&", AND},
	{"||", OR},
	{"(", OPEN_PAREN},
	{")", CLOSE_PAREN},
	{">", REDIRECTION_OUT},
	{"<", REDIRECTION_IN},
	{">>", REDIRECTION_APPEND},
	{"<<", HEREDOC},
	{"\"", DOUBLE_QUOTED_STRING},
	{"'", SINGLE_QUOTED_STRING},
	{"$", ENV_VARIABLE},
	{NULL, WORD}
};

// int ft_input_check(char *line)
// {
//     int i;
//     int single_quotes;
//     int double_quotes;

//     i = 0;
//     single_quotes = 0;
//     double_quotes = 0;

//     while (line[i] != '\0')
// 	{
//         if (line[i] == '\'')
//             single_quotes++;
//         if (line[i] == '"')
//             double_quotes++;
//         i++;
//     }
//     if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
// 	{
//         perror("Quotations not closed");
//         return (1);
//     }
//     return (0);
// }

int ft_isspace(int c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

TokenType get_token_type(char *arg)
{
	int i;

	// // POSSIBLE TO TRIM WHITESPACES BEFORE AND AFTER THE ARGUMENT IF NEEDED
	// while (ft_isspace(*arg))
	// {
	//     arg++;
	// }

    // // Trim trailing spaces
    // char *end;

    // end = arg + ft_strlen(arg) - 1;
    // while (end > arg && ft_isspace(*end))
	// // Trim trailing spaces
	// char *end;

	// end = arg + ft_strlen(arg) - 1;
	// while (end > arg && ft_isspace(*end))
	// {
	//     end--;
	// }
	// end[1] = '\0'; // Null-terminate the trimmed string

	// // Debug print to check the trimmed argument
	// ft_printf("Trimmed arg: '%s'\n", arg);

	// Iterate through the typeMap array checking if the token string is a substring of the argument
	i = 0;
	while (typeMap[i].arg != NULL)
	{
		// Check if the token string is a substring of the argument
		if (ft_strnstr(arg, typeMap[i].arg, ft_strlen(arg)) != NULL)
		{
			return typeMap[i].type;
		}
		i++;
	}
	// Default to WORD if no other type matches
	return WORD;
}


// ADD the arguments with type information into a list
t_arg *create_arg_node(char *arg)
{
	t_arg *node = (t_arg *)malloc(sizeof(t_arg));
	if (node == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	node->arg = ft_strdup(arg);
	if (node->arg == NULL)
	{
		perror("Memory allocation failed");
		free(node);
		exit(EXIT_FAILURE);
	}
	node->type = get_token_type(arg);
	node->prev = NULL;
	node->next = NULL;
	return node;
}

// Create a double linked list of arguments
void add_arg_to_list(t_arg **head, char *arg)
{
	t_arg *new_node = create_arg_node(arg);
	if (*head == NULL) {
		*head = new_node;
		return;
	}
	t_arg *current_node = *head;
	while (current_node->next != NULL) {
		current_node = current_node->next;
	}
	current_node->next = new_node;
	new_node->prev = current_node;
}

// Print list from head to end to check if arguments are added correctly
void print_args(t_arg *head)
{
	t_arg *current_node = head;
	while (current_node != NULL)
	{
		ft_printf("Argument: %s, Type: %d\n", current_node->arg, current_node->type);
		current_node = current_node->next;
	}
}

// Free the memory allocated for the list
void free_arg_list(t_arg *head)
{
	t_arg *current_node = head;
	while (current_node != NULL) {
		t_arg *temp_node = current_node;
		current_node = current_node->next;
		free(temp_node->arg);
		free(temp_node);
	}
}

void tokenizer(char *line)
{
	char **split_args = ft_split(line, ' ');
	if (!split_args)
	{
		perror("Split failed");
		exit(EXIT_FAILURE);
	}

	t_arg *args_head = NULL;
	int i;

	i = 0;
	while (split_args[i] != NULL)
	{
		add_arg_to_list(&args_head, split_args[i]);
		i++;
	}

	// Syntax check the list FOR OPERATORS aka. types PIPE, AND, OR, REDIRECTION_OUT, REDIRECTION_IN, REDIRECTION_APPEND, HEREDOC
	if (syntax_checker(args_head) == 1)
	{
		ft_printf("Syntax checker not passed\n");
		free_arg_list(args_head);
		free(split_args);
		return;
	}

	print_args(args_head);
	free_arg_list(args_head);
	free(split_args);
}
