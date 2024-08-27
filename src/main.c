/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 20:10:59 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Function to get input from the user
char *get_input()
{
    char *line = readline("minishell$ ");
    add_history(line);
    if (line == NULL)
    {
        printf("exit\n");
        exit(0);
    }
    if (*line == '\0')
    {
        free(line);
        return NULL;
    }
    return line;
}

// Function to initialize the shell
void init_shell(t_shell *shell, char **envp)
{
    ft_gc_init(&shell->gc);
    shell->env = envp;
    shell->cmds_head = NULL;
    shell->signal_received = 0;
    set_signals_parent();
}

int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;

    t_shell shell;
    char *input;

    init_shell(&shell, envp);

    input = get_input();
	if (!input)
	{
		return 0;
	}

	// Tokenize and parse commands
    t_arg *args_head = tokenizer(&shell.gc, input);
    if (!args_head) {
        fprintf(stderr, "Failed to tokenize input\n");
    }
	else
	{
		parse_commands(&shell.gc, args_head);	
	}
	
    ft_gc_free(&shell.gc);
    return 0;
}
