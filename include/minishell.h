/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/28 21:25:51 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"

# include <stdlib.h>     // Standard C Library
# include <stdio.h>      // Standard C Library
# include <errno.h>      // Standard C Library
# include <string.h>     // Standard C Library
# include <stdbool.h>    // Standard C Library

# include <unistd.h>     // POSIX Library
# include <sys/types.h>  // POSIX Library
# include <sys/stat.h>   // POSIX Library
# include <fcntl.h>      // POSIX Library
# include <signal.h>     // POSIX Library
# include <dirent.h>     // POSIX Library

# include <term.h>       // Termcap/Terminfo Library
# include <curses.h>     // Termcap/Terminfo Library
# include <ncurses.h>    // Termcap/Terminfo Library

# include <readline/readline.h>  // GNU Readline Library
# include <readline/history.h>   // GNU Readline Library

# include <termios.h>    // POSIX Terminal I/O Library

typedef enum TokenType
{
    WORD,                // Generic word (command or argument)
    REDIRECTION_OUT,     // >
    REDIRECTION_IN,      // <
    REDIRECTION_APPEND,  // >>
    HEREDOC,             // <<
    PIPE,                // |
    AND,                 // &&
    OR,                  // ||
    OPEN_PAREN,          // (
    CLOSE_PAREN,         // )
    DOUBLE_QUOTED_STRING, // Double-quoted string
    SINGLE_QUOTED_STRING, // Single-quoted string
    ENV_VARIABLE,        // Environment variable (like $HOME)
    END                  // End of input
} TokenType;

// Argument struct
typedef struct s_arg
{
    char *arg;
    enum TokenType type;
    struct s_arg *prev;
    struct s_arg *next;
} t_arg;

// Token struct
typedef struct {
    char *arg;
    TokenType type;
} Token;

int		logical_syntax(t_arg	*head);
int 	pipe_syntax(t_arg *head);
int		redirection_syntax(t_arg *head);
int		ft_isoperator(TokenType type);
int		syntax_checker(t_arg *head);
void	tokenizer(char *line);

#endif
