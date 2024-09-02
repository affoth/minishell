/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:05:34 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/02 18:34:16 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Mapping struct for token types that are required by subject pdf
const Token typeMap[] =
{
    {"|", PIPE},
    {">", REDIRECTION_OUT},
    {"<", REDIRECTION_IN},
    {">>", REDIRECTION_APPEND},
    {"<<", HEREDOC},
    {"\"", DOUBLE_QUOTED_STRING},
    {"'", SINGLE_QUOTED_STRING},
    {"$", ENV_VARIABLE},
    {NULL, WORD}
};

int ft_isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

TokenType get_token_type(char *arg)
{
    int i;

    // Special cases
    if (ft_strcmp(arg, "<<") == 0)
        return HEREDOC;
    if (ft_strcmp(arg, ">>") == 0)
        return REDIRECTION_APPEND;

    // Default cases
    i = 0;
    while (typeMap[i].arg != NULL)
    {
        if (ft_strcmp(arg, typeMap[i].arg) == 0)
            return typeMap[i].type;
        i++;
    }

    // Flags or other unrecognized tokens
    if (arg[0] == '-')
        return FLAGS;

    return WORD;
}

// Create a new argument node
t_arg *create_arg_node(t_gc *gc, char *arg)
{
    t_arg *node = (t_arg *)ft_gc_malloc(gc, sizeof(t_arg));
    if (node == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->arg = ft_shell_strdup(gc, arg);
    if (node->arg == NULL)
    {
        perror("Memory allocation failed");
        ft_gc_free(gc);  // Ensure this frees all relevant memory
        exit(EXIT_FAILURE);
    }
    node->type = get_token_type(arg);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// Add the argument to the end of the list
void add_arg_to_list(t_gc *gc, t_arg **head, char *arg)
{
    t_arg *new_node = create_arg_node(gc, arg);
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    t_arg *current_node = *head;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    current_node->next = new_node;
    new_node->prev = current_node;
}

// Tokenize the input line into arguments
t_arg *tokenizer(t_gc *gc, char *line)
{
    if (!line)
    {
        write(STDERR_FILENO, "Error: Null input line\n", 23);
        return NULL;
    }

    if (ft_quotes_not_closed(line))
    {
        write(STDERR_FILENO, "Error: Quotes not closed\n", 25);
        return NULL;
    }

    char **split_args = ft_shell_split(gc, line, ' ');
    if (!split_args)
    {
        perror("Split failed");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    t_arg *args_head = NULL;

    while (split_args[i] != NULL)
    {
        // Skip empty arguments
        if (ft_strlen(split_args[i]) > 0)
        {
            add_arg_to_list(gc, &args_head, split_args[i]);
        }
        i++;
    }

    // Syntax check the list
    if (syntax_checker(args_head) != 0)
    {
        write(STDERR_FILENO, "Error: Syntax checker not passed\n", 33);
        return NULL;
    }

    return args_head;
}
