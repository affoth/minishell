/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:09:46 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/15 15:52:10 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Mapping struct for token types that are required by subject pdf
const Token typeMap[] = {
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

TokenType get_token_type(const char *arg)
{
    if (strcmp(arg, "<<") == 0)
        return HEREDOC;
    if (strcmp(arg, ">>") == 0)
        return REDIRECTION_APPEND;
    if (strcmp(arg, "<") == 0)
        return REDIRECTION_IN;
    if (strcmp(arg, ">") == 0)
        return REDIRECTION_OUT;

    for (int i = 0; typeMap[i].arg != NULL; i++)
    {
        if (strcmp(arg, typeMap[i].arg) == 0)
            return typeMap[i].type;
    }

    if (arg[0] == '-' && arg[1] != '\0' && isalpha((unsigned char)arg[1]))
        return FLAGS;

    return WORD;
}

t_arg *create_arg_node(t_gc *gc, const char *arg)
{
    t_arg *node = ft_gc_malloc(gc, sizeof(t_arg));  // Adjust memory management according to your GC
    if (node == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->arg = strdup(arg);  // Adjust memory management according to your GC
    if (node->arg == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->type = get_token_type(arg);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void add_arg_to_list(t_gc *gc, t_arg **head, const char *arg)
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

void print_tokens(t_arg *head)
{
    t_arg *current = head;
    while (current != NULL)
    {
        printf("Token: %s, Type: %d\n", current->arg, current->type);
        current = current->next;
    }
}

t_arg *tokenizer(t_shell *shell, char *input)
{
    if (!input)
    {
        write(STDERR_FILENO, "Error: Null input line\n", 23);
        shell->exit_status = 1;
        return NULL;
    }

    // Split the input string into tokens
    char **tokens = ft_split_redirections(&shell->gc, input);
    if (!tokens)
    {
        write(STDERR_FILENO, "Error: Memory allocation failed\n", 32);
        shell->exit_status = 1;
        return NULL;
    }

    t_arg *args_head = NULL;
    for (size_t i = 0; tokens[i] != NULL; i++)
    {
        add_arg_to_list(&shell->gc, &args_head, tokens[i]);
    }

    // Optionally print tokens for debugging
    print_tokens(args_head);

    return args_head;
}
