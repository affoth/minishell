/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 17:32:10 by mokutucu         ###   ########.fr       */
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
# include <sys/ioctl.h>  // POSIX Library
# include <fcntl.h>      // POSIX Library
# include <signal.h>     // POSIX Library
# include <bits/sigaction.h> // POSIX Library
# include <dirent.h>     // POSIX Library

# include <term.h>       // Termcap/Terminfo Library
# include <curses.h>     // Termcap/Terminfo Library
# include <ncurses.h>    // Termcap/Terminfo Library

# include <readline/readline.h>  // GNU Readline Library
# include <readline/history.h>   // GNU Readline Library

# include <termios.h>    // POSIX Terminal I/O Library

extern char		**environ;

// TokenType enum
typedef enum TokenType
{
    WORD,                // Generic word (command or argument)
    REDIRECTION_OUT,     // >
    REDIRECTION_IN,      // <
    REDIRECTION_APPEND,  // >>
    HEREDOC,             // <<
    PIPE,                // |
    DOUBLE_QUOTED_STRING, // Double-quoted string
    SINGLE_QUOTED_STRING, // Single-quoted string
    ENV_VARIABLE,        // Environment variable (like $HOME)
    END                  // End of input
} TokenType;

// Argument struct
typedef struct s_arg
{
    char *arg;
    TokenType type;
    struct s_arg *prev;
    struct s_arg *next;
} t_arg;

// Command struct
typedef struct s_command
{
    t_arg *args_head;    // Head of the arguments list for this command
    struct s_command *next; // Pointer to the next command in the pipeline
    int stdin_fd;        // File descriptor for stdin redirection
    int stdout_fd;       // File descriptor for stdout redirection
} t_command;

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

typedef struct s_gc
{
    t_garbage *head;
} t_gc;

// Main shell struct
typedef struct s_shell
{
    t_gc gc;            // Garbage collector
    char **env;         // Environment variables
    t_command *cmds_head; // Head of the commands list
    volatile sig_atomic_t signal_received; // Signal handling
} t_shell;

// Function prototypes for garbage collector
void ft_gc_init(t_gc *gc);
void ft_gc_free(t_gc *gc);
void *ft_gc_malloc(t_gc *gc, size_t size);

// Function prototypes for string manipulation
char *ft_shell_strdup(t_gc *gc, const char *s);
char *ft_shell_strndup(t_gc *gc, const char *s, size_t n);
char *ft_shell_substr(t_gc *gc, const char *s, unsigned int start, size_t len);
char *ft_shell_strjoin(t_gc *gc, char *s1, char *s2);
char **ft_shell_split(t_gc *gc, const char *s, char c);
// Function prototypes for parsing
void handle_quote_split(const char *s, size_t i, bool *quote);
void skip_quoted_string(const char **s, bool *quote, char *quote_char);
void assign(size_t *i, size_t *j, int *index, bool *quote);
int ft_quotes_not_closed(char *line);

// Expansion
char *expand_string(t_gc *gc, char *input);

// Syntax analysis
int syntax_checker(t_arg *head);
int pipe_syntax(t_arg *head);
int	redirection_syntax(t_arg *head);
int word_syntax(t_arg *head);

// Function prototypes for command processing
t_arg *tokenizer(t_gc *gc, char *input);
t_command *create_command(t_gc *gc);
void add_arg_to_command(t_command *cmd, t_arg *arg);
t_command *parse_commands(t_gc *gc, t_arg *args_head);
void print_commands(t_command *cmds_head);

// Function prototypes for built-in commands
int is_built_in(char *cmd);
void exec_built_ins(t_shell *shell);
void built_in_cd(t_shell *shell);
void built_in_pwd(void);
void built_in_env(char **env);
void built_in_echo(t_arg *args_head);
int ft_env_len(char **env);
char *find_variable(t_gc *gc, const char *arg);
int find_var_in_env(char **env, const char *var_name);
char **add_env_var(t_gc *gc, char *arg, char **env, int env_len);
char **change_or_add_env_var(t_gc *gc, char *arg, char **env);
void built_in_export(t_gc *gc, t_arg *args_head, char ***env);
void built_in_unset(t_gc *gc, t_arg *args_head, char ***env);
void built_in_exit(t_arg *args_head);

// Function prototypes for execve
void execve_args(t_shell *shell);
char *get_path(t_gc *gc, char *cmd);
int count_arguments(t_arg *args_head);
char *remove_quotes(t_gc *gc, const char *str);

// Signal handling
void sigint_handler(int signum);
void set_signals_parent(void);
void set_signals_child(void);

#endif // MINISHELL_Hz