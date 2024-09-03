#include "../../include/minishell.h"

// Helper function to remove an environment variable from the environment array
char **remove_env_var(t_gc *gc, char **env, int index)
{
	int i = 0;
	int j = 0;
	int env_len = ft_env_len(env);
	char **new_env = ft_gc_malloc(gc, sizeof(char *) * env_len);

	if (!new_env)
	{
		ft_printf("Error: Memory allocation failed\n");
		return env;
	}

	while (i < env_len)
	{
		if (i != index)
		{
			new_env[j] = env[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;

	return new_env;
}

// Function to remove an environment variable
void built_in_unset(t_shell *shell)
{
	t_gc *gc;
	char **args;
	char **env;
	int var_index;

	gc = &shell->gc;
	env = shell->env;
	args = shell->cmds_head->args;  // Get the arguments array

	if (!args[0])  // Check if there are arguments to unset
	{
		ft_printf("unset: not enough arguments\n");
		return;
	}

	int i = 0;
	while (args[i])
	{
		var_index = find_var_in_env(env, args[i]);
		if (var_index != -1)
		{
			// Remove the environment variable
			env = remove_env_var(gc, env, var_index);
		}
		else
		{
			ft_printf("unset: `%s': not a valid identifier\n", args[i]);
		}
		i++;
	}

	shell->env = env; // Update the shell's environment pointer
}
