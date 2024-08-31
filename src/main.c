/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/08/31 14:02:53 by mokutucu         ###   ########.fr       */
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

void print_cmd_args(char **args, const char *label) {
    if (args) {
        printf("%s:\n", label);
        for (int i = 0; args[i]; i++) {
            printf("  Arg %d: %s\n", i, args[i]);
        }
    } else {
        printf("%s: NULL\n", label);
    }
}

void print_commands(t_command *cmds_head) {
    t_command *cmd = cmds_head;

    while (cmd) {
        printf("Command:\n");
        printf("  stdin_fd: %d\n", cmd->stdin_fd);
        printf("  stdout_fd: %d\n", cmd->stdout_fd);
        printf("  Append mode: %s\n", cmd->append_mode ? "true" : "false");
        printf("  Command name: %s\n", cmd->cmd_name);
        
        print_cmd_args(cmd->flags, "Flags");
        print_cmd_args(cmd->args, "Args");
        
        cmd = cmd->next;
        printf("-----\n");
    }
}

// Function to clean up and print commands
void execute_shell(t_shell *shell) {
    char *input;
    t_arg *args_head;
    t_command *cmds_head;
    int pipe_count;

    while (1) // Main loop for shell
    {
        input = get_input(); // Get user input
        if (!input) // If input is NULL (empty line), continue the loop
        {
            free(input);
            continue;
        }

        // Tokenize and parse commands
        args_head = tokenizer(&shell->gc, input);
        pipe_count = count_pipes(args_head);
        cmds_head = create_and_populate_commands(&shell->gc, args_head, pipe_count);

        // Debugging: print the commands
        print_commands(cmds_head);

        // Commented out the execution of commands
        
        if (cmds_head)
        {
            if (needs_piping(cmds_head))
            {
                execute_commands_with_pipes(shell, cmds_head);
            }
            else
            {
                execute_commands_without_pipes(shell, cmds_head);
            }
        }
        

        // Free allocated memory for arguments and commands
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

    // Clean up
    ft_gc_free(&shell.gc);

    return 0;
}
