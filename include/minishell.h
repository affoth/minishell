/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/07/04 17:06:01 by afoth            ###   ########.fr       */
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
# include <sys/wait.h>   // POSIX Library
# include <fcntl.h>      // POSIX Library
# include <signal.h>     // POSIX Library
# include <dirent.h>     // POSIX Library

# include <term.h>       // Termcap/Terminfo Library
# include <curses.h>     // Termcap/Terminfo Library
# include <ncurses.h>    // Termcap/Terminfo Library

# include <readline/readline.h>  // GNU Readline Library
# include <readline/history.h>   // GNU Readline Library

# include <termios.h>    // POSIX Terminal I/O Library

extern char **environ;

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
	int exit_status;
	//int fd[2];
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
void	*ft_gc_malloc(size_t size);
void	ft_gc_free(void);

//split
void	handle_quote_split(const char *s, size_t i, bool *quote);
void	skip_quoted_string(const char **s, bool *quote, char *quote_char);
void	assign(size_t *i, size_t *j, int *index, bool *quote);
char	**ft_shell_split(char const *s, char c);
int		ft_quotes_not_closed(char *line);

//expansion
char	*expand_string(char *input);

//lexer
char	*ft_shell_strdup(const char *s1);
char	*ft_shell_strndup(const char *s1, size_t n);
char	*ft_shell_strjoin(char *s1, char *s2);
t_arg	*tokenizer(char *line);

//syntax
int		word_syntax(t_arg *head);
int		logical_syntax(t_arg	*head);
int		pipe_syntax(t_arg *head);
int		redirection_syntax(t_arg *head);
int		ft_isoperator(TokenType type);
int		syntax_checker(t_arg *head);

//added by afoth
char	*ft_expand_env(char *env);
char	*ft_shell_strjoin(char *s1, char *s2);
char	*ft_shell_substr(const char *s, unsigned int start, size_t len);

//sinple redirections
void	simple_append_redirection(t_arg *head, t_arg *tmp);
void	simple_output_redirection(t_arg *head, t_arg *tmp);
void	simple_input_redirection(t_arg *head, t_arg *tmp);
void	simple_pipe_redirection(t_arg *head, t_arg *tmp);

//redirections and pipes
void	handle_redirection_or_pipe(t_arg *head_position);
int		input_redirection(t_arg *head, t_arg *tmp);
int		check_file_readable(const char *filepath);
void	output_redirection(t_arg *first_arg, t_arg *second_arg, int fd);
void	append_redirection(t_arg *head, t_arg *tmp, int fd);
void	heredoc(t_arg *head, t_arg *tmp);
int		find_redirections_and_pipes(t_arg *head);
int		redirect_count_arguments(t_arg *args_head);
void	redirect_execve_args(t_arg *args_head);
void	multiple_redirections(t_arg *head);
int		handle_multiple_redirections_and_pipes(t_arg *first_arg, t_arg *second_arg, int fd);
int		is_executable(t_arg *arg);
t_arg	*search_for_next_redirection(t_arg *tmp);

//pipes
void	pipe_redirection(t_arg *head, t_arg *tmp, int fd_input);
int		multiple_pipes(t_arg *first_arg, t_arg *second_arg, int fd);
void	process_left_arg(int *fd, t_arg *first_arg);
void	process_right_arg(int *fd, t_arg *second_arg);
void	process_right_arg_with_pipes_ahead(int *fd, int *fd2, t_arg *second_arg);
int		redirection_ahead(t_arg *second_arg);

//built_ins
int		is_built_in(char *cmd);
void	exec_built_ins(t_arg *args_head);
void	built_in_cd(t_arg *args_head, char ***env);
void	built_in_pwd(void);
void	built_in_env(char **env);
void	built_in_echo(t_arg *args_head);
int		ft_env_len(char **env);
char	*find_variable(const char *arg);
int		find_var_in_env(char **env, const char *var_name);
char	**add_env_var(char *arg, char **env, int env_len);
char	**change_or_add_env_var(char *arg, char **env);
void	built_in_export(t_arg *args_head, char ***env);
void	built_in_unset(t_arg *args_head, char ***env);
void	built_in_exit(t_arg *args_head);

//execve
void	execve_args(t_arg *args_head);
char	*get_path(char *cmd);
int		count_arguments(t_arg *args_head);
char	*remove_quotes(const char *str);


//signals
void	signal_init();
void	sigint_handler(int signal);

#endif
