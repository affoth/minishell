/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 20:07:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 18:12:27 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	ft_words(char const *s, char c)
{
    size_t count = 0;
    bool in_quote = false;
    char quote_char = 0;

    while (*s)
    {
        if (*s == '\'' || *s == '\"')
        {
            if (!in_quote)
            {
                in_quote = true;
                quote_char = *s;
            }
            else if (*s == quote_char)
            {
                in_quote = false;
                quote_char = 0;
            }
        }
        if (*s == c && !in_quote)
        {
            while (*s == c)
                s++;
            count++;
        }
        else
            s++;
    }
    return count + (in_quote ? 1 : 0);
}
static void	*ft_allocate(t_gc *gc, const char *s, int start, int end)
{
    char *word = ft_gc_malloc(gc, end - start + 1);
    if (!word)
        return NULL;
    strncpy(word, s + start, end - start);
    word[end - start] = '\0';
    return word;
}

char	**ft_shell_split(t_gc *gc, const char *s, char c)
{
    char	**array;
    size_t	i;
    size_t	j;
    bool	quote;
    int		index;

    if (!s)
        return NULL;

    array = ft_gc_malloc(gc, (ft_words(s, c) + 1) * sizeof(char *));
    if (!array)
        return NULL;

    assign(&i, &j, &index, &quote);
    while (i <= ft_strlen(s))
    {
        handle_quote_split(s, i, &quote);
        if ((s[i] != c || quote == true) && index < 0)
            index = i;
        else if (((s[i] == c && quote == false) || i == ft_strlen(s)) && index >= 0)
        {
            array[j++] = ft_allocate(gc, s, index, i);
            index = -1;
        }
        i++;
    }
    array[j] = NULL;
    return array;
}
