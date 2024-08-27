/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/27 22:31:53 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Function to get input from the user
char *get_input()
{
    char *line = readline("minishell$ ");
    if (line == NULL) // Handle EOF or input errors
    {
        printf("exit\n");
        exit(0);
    }
    if (*line == '\0') // Handle empty input
    {
        free(line);
        return NULL;
    }
    add_history(line);
    return line;
}

// Function to initialize the shell
void init_shell(t_shell *shell, char **envp)
{
    ft_gc_init(&shell->gc);
    shell->env = envp;
    shell->cmds_head = NULL;
    shell->signal_received = 0;
    set_signals_parent(); // Ensure signal handling is properly set
}

// Function to clean up and execute commands
void execute_shell(t_shell *shell) {
    char *input;
    t_arg *args_head;
    t_command *cmds_head;

    while (1) // Main loop for shell
    {
        input = get_input(); // Get user input
        if (!input) // If input is NULL (empty line), continue the loop
            continue;

        // Tokenize and parse commands
        args_head = tokenizer(&shell->gc, input);
        cmds_head = parse_commands(&shell->gc, args_head);

        // Execute commands based on whether piping is needed
        if (cmds_head) {
            printf("Executing commands\n");
            if (needs_piping(cmds_head)) {
                execute_commands_with_pipes(shell, cmds_head);
            } else {
                execute_commands_without_pipes(shell, cmds_head);
            }
        }

        // Clean up
        ft_gc_free(&shell->gc);
        free(input);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_shell shell;

    init_shell(&shell, envp);
    execute_shell(&shell); // Main shell execution loop

    return 0;
}
