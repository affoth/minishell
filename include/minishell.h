/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/31 19:23:50 by afoth            ###   ########.fr       */
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

// Garbage collector struct
typedef struct s_garbage
{
	void *ptr;
	struct s_garbage *next;
} t_garbage;

//garbage collector
void    *ft_gc_malloc(size_t size);
void    ft_gc_free(void);

//split
void	handle_quote_split(const char *s, size_t i, bool *quote);
void	skip_quoted_string(const char **s, bool *quote, char *quote_char);
void	assign(size_t *i, size_t *j, int *index, bool *quote);
char	**ft_shell_split(char const *s, char c);
int		ft_quotes_not_closed(char *line);

//lexer
char	*ft_shell_strdup(const char *s1);
void	tokenizer(char *line);
//syntax
int		word_syntax(t_arg *head);
int		logical_syntax(t_arg	*head);
int		pipe_syntax(t_arg *head);
int		redirection_syntax(t_arg *head);
int		ft_isoperator(TokenType type);
int		syntax_checker(t_arg *head);

//added by afoth
char	*ft_expand_env(char *env);
void	handle_expansions(t_arg *head);
void	memory_error(void);
int		find_end_of_env_in_quotes(char *arg, int i);
char	*ft_shell_strjoin(char *s1, char *s2);
char	*ft_shell_substr(const char *s, unsigned int start, size_t len);
void	*ft_shell_calloc(size_t count, size_t size);
#endif
