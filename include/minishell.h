/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:36:35 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/16 18:17:25 by mokutucu         ###   ########.fr       */
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

// Define exit status codes
#define EXIT_COMMAND_NOT_FOUND 127
#define EXIT_PERMISSION_DENIED 126
#define EXIT_INVALID_COMMAND 2
#define EXIT_FORK_FAILED 1
#define EXIT_EXECVE_FAILED 1
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

extern char		**environ;

// Forward declaration of t_shell for gc
typedef struct s_shell t_shell;

// TokenType enum
typedef enum TokenType
{
    WORD,                // Generic word (command or argument)
    FLAGS,                // Command flag (e.g., -l)
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
    char *cmd_name;          // Command name (e.g., "ls")
    char **flags;         // Command flags (e.g., "-l")
    char **args;          // Command arguments (e.g., "file.txt")
    struct s_command *next;  // Next command in the pipeline
    int stdin_fd;            // File descriptor for stdin redirection
    int stdout_fd;           // File descriptor for stdout redirection
    bool append_mode;        // Flag for append mode (1 for >>, 0 for >)
    bool valid; // Indicates if the command should be executed

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

// Garbage collector struct with shell pointer
typedef struct s_gc
{
    t_garbage *head;
    t_shell *shell;  // Pointer to the shell struct
} t_gc;

// Main shell struct
typedef struct s_shell
{
    t_gc gc;                // Garbage collector
    char **env;             // Environment variables
    t_command *cmds_head;   // Head of the commands list
    int exit_status;        // Exit status of the shell
    volatile sig_atomic_t signal_received; // Signal handling
} t_shell;

// Function prototypes for garbage collector
void ft_gc_init(t_gc *gc, t_shell *shell);
void ft_gc_free(t_gc *gc);
void *ft_gc_malloc(t_gc *gc, size_t size);
void *ft_gc_realloc(t_gc *gc, void *ptr, size_t old_size, size_t new_size);

// Function prototypes for string manipulation
char *ft_shell_strdup(t_gc *gc, const char *s);
char *ft_shell_strndup(t_gc *gc, const char *s, size_t n);
char *ft_shell_substr(t_gc *gc, const char *s, unsigned int start, size_t len);
char *ft_shell_strjoin(t_gc *gc, char *s1, char *s2);
// split stuff
size_t	ft_words(char const *s, char c);
char **ft_shell_split(t_gc *gc, const char *s, char c);
char **ft_split_redirections(t_gc *gc, const char *s);

// Function prototypes for parsing
void handle_quote_split(const char *s, size_t i, bool *quote);
void skip_quoted_string(const char **s, bool *quote, char *quote_char);
void assign(size_t *i, size_t *j, int *index, bool *quote);
int ft_quotes_not_closed(const char *line);

// Expand environment variables
char *expand_string(t_gc *gc, char *input, int exit_status);

// Syntax analysis
int syntax_checker(t_arg *head);
int pipe_syntax(t_arg *head);
int redirection_syntax(t_arg *head);
int word_syntax(t_arg *head);

// Function prototypes for command processing
t_arg *tokenizer(t_shell *shell, char *input);
t_command *create_command(t_gc *gc);
void add_arg_to_command(t_command *cmd, const char *arg, t_gc *gc);
void set_command_name(t_command *cmd, const char *name, t_gc *gc);
int count_pipes_argstruct(t_arg *args_head);
int count_pipes_cmdstruct(t_command *cmds_head);
t_command *create_and_populate_commands(t_gc *gc, t_arg *args_head, int pipe_count);
void print_commands(t_command *cmds_head);


// Function prototypes for redirection handling
int handle_output_redirection(t_command *cmd, t_arg *arg);
int handle_input_redirection(t_command *cmd, t_arg *arg);
bool parse_heredoc(t_command *cmd, t_arg *arg);

// Function prototypes for built-in commands
int is_built_in(char *cmd);
int exec_built_ins(t_shell *shell, t_command *cmd);
int built_in_cd(t_shell *shell);
int built_in_pwd(void);
int built_in_env(t_shell *shell);
int built_in_echo(t_shell *shell, t_command *cmd);
int ft_env_len(char **env);
char *find_variable(t_gc *gc, const char *arg);
int find_var_in_env(char **env, const char *var_name);
char **add_env_var(t_gc *gc, char *arg, char **env, int env_len);
char **change_or_add_env_var(t_gc *gc, char *arg, char **env);
int built_in_export(t_shell *shell);
int built_in_unset(t_shell *shell);
int built_in_exit(t_shell *shell);

// Function prototypes for executing commands
int execute_command_without_pipes(t_shell *shell, t_command *cmds_head);
int execute_commands_with_pipes(t_shell *shell, t_command *cmds_head);

int execute_command(t_shell *shell, t_command *cmd);
int fork_and_execute_command(t_shell *shell, t_command *cmd, int *pipe_descriptors, int cmd_index, int num_pipes);

char *remove_quotes(t_gc *gc, const char *str);
char *get_path(t_gc *gc, char *cmd);
int count_arguments(char **args);
int needs_piping(t_command *cmds_head);
void setup_redirections(int cmd_index, int num_pipes, int *pipe_descriptors);
void create_pipes(int num_pipes, int *pipe_descriptors);
void close_pipes(int num_pipes, int *pipe_descriptors);

// Signal handling
void setup_signals(void);
void setup_child_signals(void);
void handle_signal(int sig);
void child_handle_signal(int sig);



#endif // MINISHELL_H
