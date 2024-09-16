/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:35:01 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 21:19:08 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * Function to remove quotes from a string.
 * It removes both single and double quotes while keeping the content inside intact.
 */
static char *strip_quotes(t_gc *gc, const char *str)
{
    char *result;
    size_t i = 0, j = 0;
    bool in_single_quote = false, in_double_quote = false;

    result = ft_gc_malloc(gc, ft_strlen(str) + 1);
    if (!result)
        return NULL;

    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (str[i] == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else
        {
            result[j++] = str[i];
        }
        i++;
    }
    result[j] = '\0';
    return result;
}

/*
 * Handles the redirection operators, tokenizes them, and manages quotes.
 */
static void ft_handle_redirection(t_gc *gc, const char *s, char **array, size_t *index)
{
    size_t i = 0;
    bool in_quote = false;
    char quote_char = 0;
    size_t start = 0;

    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '\"')
        {
            if (!in_quote)
            {
                in_quote = true;
                quote_char = s[i];
            }
            else if (s[i] == quote_char)
            {
                in_quote = false;
                quote_char = 0;
            }
        }

        // Detect and split redirection operators and pipes
        if ((s[i] == '<' || s[i] == '>' || s[i] == '|') && !in_quote)
        {
            if (i > start) // Add previous token before the special operator
            {
                char *token = ft_shell_strndup(gc, s + start, i - start);
                array[*index] = strip_quotes(gc, token); // Remove quotes
                (*index)++;
            }

            // Handle double redirection (<< or >>)
            if (s[i] == '<' || s[i] == '>')
            {
                if (s[i + 1] == s[i])
                {
                    array[*index] = ft_gc_malloc(gc, 3); // "<< " or ">> "
                    strncpy(array[*index], s + i, 2);
                    array[*index][2] = '\0';
                    (*index)++;
                    i++;
                }
                else // Handle single redirection (< or >)
                {
                    array[*index] = ft_gc_malloc(gc, 2); // "< " or "> "
                    strncpy(array[*index], s + i, 1);
                    array[*index][1] = '\0';
                    (*index)++;
                }
            }
            else if (s[i] == '|') // Handle pipe
            {
                array[*index] = ft_gc_malloc(gc, 2); // "| "
                strncpy(array[*index], s + i, 1);
                array[*index][1] = '\0';
                (*index)++;
            }

            start = i + 1; // Update start for next token
        }
        else if (s[i] == ' ' && !in_quote)
        {
            if (i > start) // Add token before space
            {
                char *token = ft_shell_strndup(gc, s + start, i - start);
                array[*index] = strip_quotes(gc, token); // Remove quotes
                (*index)++;
            }
            start = i + 1; // Update start for next token
        }
        i++;
    }

    // Add the last token if needed
    if (i > start)
    {
        char *token = ft_shell_strndup(gc, s + start, i - start);
        array[*index] = strip_quotes(gc, token); // Remove quotes
        (*index)++;
    }
}


/*
 * Splits the input string into tokens while handling redirections and quotes.
 */
char **ft_split_redirections(t_gc *gc, const char *s)
{
    char **array;
    size_t num_tokens;
    size_t index = 0;

    if (!s)
        return NULL;

    num_tokens = ft_words(s, ' ') + 10; // Add some buffer for additional tokens (like redirection)
    array = ft_gc_malloc(gc, num_tokens * sizeof(char *));
    if (!array)
        return NULL;

    ft_handle_redirection(gc, s, array, &index);

    // NULL-terminate the array
    array[index] = NULL;
    return array;
}
