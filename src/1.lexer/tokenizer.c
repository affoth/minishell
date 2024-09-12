/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:09:46 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/12 16:58:03 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Mapping struct for token types that are required by subject pdf
const Token typeMap[] = {
    {"|", PIPE},
    {"\"", DOUBLE_QUOTED_STRING},
    {"'", SINGLE_QUOTED_STRING},
    {"$", ENV_VARIABLE},
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

    // This function splits the input into tokens by spaces
    char **split_args = ft_shell_split(&shell->gc, input, ' ');
    if (!split_args)
    {
        perror("Split failed");
        shell->exit_status = 1;
        return NULL;
    }

    int i = 0;
    t_arg *args_head = NULL;

    while (split_args[i] != NULL)
    {
        // Skip empty arguments
        if (ft_strlen(split_args[i]) > 0)
        {
            // Handle potential redirection tokens and their associated file names
            char *arg = split_args[i];
            int length = ft_strlen(arg);

            // Split based on redirection characters
            if (arg[0] == '<' || arg[0] == '>')
            {
                if (length > 1)
                {
                    // Handle the case where there is more than one character after '<' or '>'
                    char redirection_char[2] = { arg[0], '\0' };
                    add_arg_to_list(&shell->gc, &args_head, redirection_char);

                    char *remaining = arg + 1;
                    if (ft_strlen(remaining) > 0)
                    {
                        add_arg_to_list(&shell->gc, &args_head, remove_quotes(&shell->gc, remaining));
                    }
                }
                else
                {
                    // Handle single character redirection tokens
                    add_arg_to_list(&shell->gc, &args_head, arg);
                }
                i++;
            }
            else
            {
                add_arg_to_list(&shell->gc, &args_head, split_args[i]);
                i++;
            }
        }
        else
        {
            i++;
        }
    }

    print_tokens(args_head);

    return args_head;
}