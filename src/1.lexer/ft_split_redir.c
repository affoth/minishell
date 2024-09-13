/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:35:01 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/13 18:47:28 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

        // Detect and split redirection operators
        if ((s[i] == '<' || s[i] == '>') && !in_quote)
        {
            if (i > start) // Add previous token before the redirection operator
            {
                array[*index] = ft_gc_malloc(gc, i - start + 1);
                if (!array[*index])
                    return; // Handle allocation failure
                strncpy(array[*index], s + start, i - start);
                array[*index][i - start] = '\0';
                (*index)++;
            }
            // Handle double redirection (<< or >>)
            if (s[i + 1] == s[i])
            {
                array[*index] = ft_gc_malloc(gc, 3); // "<< " or ">> "
                if (!array[*index])
                    return; // Handle allocation failure
                strncpy(array[*index], s + i, 2);
                array[*index][2] = '\0';
                (*index)++;
                i++;
            }
            else // Handle single redirection (< or >)
            {
                array[*index] = ft_gc_malloc(gc, 2); // "< " or "> "
                if (!array[*index])
                    return; // Handle allocation failure
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
                array[*index] = ft_gc_malloc(gc, i - start + 1);
                if (!array[*index])
                    return; // Handle allocation failure
                strncpy(array[*index], s + start, i - start);
                array[*index][i - start] = '\0';
                (*index)++;
            }
            start = i + 1; // Update start for next token
        }
        i++;
    }

    // Add the last token if needed
    if (i > start)
    {
        array[*index] = ft_gc_malloc(gc, i - start + 1);
        if (!array[*index])
            return; // Handle allocation failure
        strncpy(array[*index], s + start, i - start);
        array[*index][i - start] = '\0';
        (*index)++;
    }
}

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
