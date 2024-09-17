/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:27:20 by mokutucu          #+#    #+#             */
/*   Updated: 2024/09/17 23:44:32 by afoth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Swap two strings in an array
void	ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

// Selection sort to sort environment variables alphabetically
void	ft_sort(char **env, int len)
{
	int	i;
	int	j;
	int	min_idx;

	i = 0;
	while (i < len - 1)
	{
		min_idx = i;
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(env[j], env[min_idx]) < 0)
			{
				min_idx = j;
			}
			j++;
		}
		ft_swap(&env[i], &env[min_idx]);
		i++;
	}
}

void	print_export_env(t_gc *gc, char **env)
{
	int		env_len;
	char	**sorted_env;
	int		i;

	env_len = ft_env_len(env);
	sorted_env = (char **)ft_gc_malloc(gc, sizeof(char *) * (env_len + 1));
	if (!sorted_env)
	{
		perror("Failed to allocate memory for sorted environment");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < env_len)
	{
		sorted_env[i] = ft_shell_strdup(gc, env[i]);
		i++;
	}
	sorted_env[env_len] = NULL;
	ft_sort(sorted_env, env_len);
	i = 0;
	while (i < env_len)
	{
		ft_printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
}

// Helper function to check if a string
// is a valid environment variable identifier
int	is_valid_identifier(const char *str)
{
	if (!str || !*str || (!ft_isalpha((unsigned char)*str) && *str != '_'))
		return (0);
	while (*str)
	{
		if (!ft_isalnum((unsigned char)*str) && *str != '_')
			return (0); // Must be alphanumeric or underscore
		str++;
	}
	return (1);
}

// Export built-in command
int	built_in_export(t_shell *shell)
{
	t_gc *gc;
	char **args;
	char **env;
	int i;
	int has_errors;

	// Extract the environment and arguments
	gc = &shell->gc;
	env = shell->env; // Directly use the environment
	args = shell->cmds_head->args;

	// If there are no additional arguments, print the environment
	if (!args || !args[0])
	{
		print_export_env(gc, env);
		return 0; // Success
	}

	// Initialize error flag
	has_errors = 0;

	// Process each argument
	i = 0; // Start from the first argument (skipping the command itself)
	while (args[i])
	{
		char *arg = args[i];

		// Validate and update environment variable
		if (arg && ft_strchr(arg, '='))
		{
			if (arg[0] == '=' || arg[ft_strlen(arg) - 1] == '=')
			{
				// Invalid format if it starts or ends with '='
				ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
				has_errors = 1; // Indicate an error
			}
			else
			{
				char *var_name = find_variable(gc, arg);
				if (is_valid_identifier(var_name))
				{
					env = change_or_add_env_var(gc, arg, env); // Update env directly
				}
				else
				{
					ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
					has_errors = 1; // Indicate an error
				}
				free(var_name);
			}
		}
		else
		{
			// If no '=' in argument, treat it as a variable to set with an empty value
			if (is_valid_identifier(arg))
			{
				char *export_arg = ft_shell_strjoin(gc, arg, "=");
				export_arg = ft_shell_strjoin(gc, export_arg, "");
				env = change_or_add_env_var(gc, export_arg, env); // Update env directly
				free(export_arg);
			}
			else
			{
				ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
				has_errors = 1; // Indicate an error
			}
		}
		i++; // Move to the next argument
	}
	shell->env = env; // Ensure the shell's environment is updated

	return has_errors; // Return 1 if there were errors, 0 otherwise
}
