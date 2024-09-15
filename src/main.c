/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:58:44 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/15 14:19:39 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **init_env(char **envp, t_gc *gc)
{
    char **env_copy;
    int env_len;
    int i;

    // Calculate the length of the environment array
    env_len = ft_env_len(envp);

    // Allocate memory for the copy
    env_copy = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
    if (!env_copy)
    {
        perror("Failed to allocate memory for environment copy");
        exit(EXIT_FAILURE);
    }

    // Copy the environment variables
    i = 0;
    while (i < env_len)
    {
        env_copy[i] = ft_shell_strdup(gc, envp[i]);
        i++;
    }
    env_copy[env_len] = NULL;

    return env_copy;
}

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

void init_shell(t_shell *shell, char **envp)
{
    ft_gc_init(&shell->gc, shell);

    // Initialize the environment
    shell->env = init_env(envp, &shell->gc);
    shell->cmds_head = NULL;
    shell->exit_status = 0;
}


void print_cmd_args(char **args, const char *label)
{
    if (args) {
        printf("%s:\n", label);
        for (int i = 0; args[i]; i++) {
            printf("  Arg %d: %s\n", i, args[i]);
        }
    } else {
        printf("%s: NULL\n", label);
    }
}

void print_commands(t_command *cmds_head)
{
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

// Function to check if piping is needed
int needs_piping(t_command *cmds_head) {
    t_command *current_cmd = cmds_head;
    while (current_cmd) {
        if (current_cmd->next) {
            return 1; // There is at least one pipe needed
        }
        current_cmd = current_cmd->next;
    }
    return 0; // No pipes needed
}

void execute_shell(t_shell *shell)
{
    char *input;
    char *expanded_vars;
    t_arg *args_head;
    int pipe_count;

    while (1) // Main loop for shell
    {
        input = get_input(); // Get user input
        if (!input) // If input is NULL (empty line), continue the loop
        {
            free(input);
            continue;
        }

        // Expand variables
        expanded_vars = expand_string(&shell->gc, input, shell->exit_status);
        // Tokenize and parse commands
        args_head = tokenizer(shell, expanded_vars);
        pipe_count = count_pipes_argstruct(args_head);
        shell->cmds_head = create_and_populate_commands(&shell->gc, args_head, pipe_count, shell);
        print_commands(shell->cmds_head);

        // Execute commands
        if (needs_piping(shell->cmds_head))
        {
            //printf("Executing commands with pipes\n");so i
            shell->exit_status = execute_commands_with_pipes(shell, shell->cmds_head);
        }
        else
        {
            //printf("Executing commands without pipes\n");
            shell->exit_status = execute_command_without_pipes(shell, shell->cmds_head);
        }
        //printf("Exit status: %d\n", shell->exit_status);
        // Free allocated memory for arguments and commands
        free(input);
        }
}


int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_shell shell;

	setup_signals();
    init_shell(&shell, envp);
    execute_shell(&shell); // Main shell execution loop

    // Clean up
    ft_gc_free(&shell.gc);

    return 0;
}
