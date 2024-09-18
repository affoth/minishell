static char	**prepare_args(t_shell *shell, t_command *cmd)
{
	int		flags_count;
	int		args_count;
	int		i;
	int		j;
	char	**args;

	flags_count = flags_count(cmd);
	args_count = args_count(cmd);
	args = (char **)ft_gc_malloc(&shell->gc, sizeof(char *) * (flags_count + args_count + 2));
	if (!args)
		perror_malloc();
	i = 0;
	while (i < flags_count)
	{
		args[i + 1] = ft_shell_strdup(&shell->gc, cmd->flags[i]);
		if (!args[i + 1])
			perror_strdup();
		i++;
	}
	args[0] = ft_shell_strdup(&shell->gc, cmd->cmd_name);
	j = 0;
	while (j < args_count)
	{
		args[flags_count + 1 + j] = remove_quotes(&shell->gc, strdup(cmd->args[j]));
		if (!args[flags_count + 1 + j])
			perror_strdup();
		j++;
	}
	args[flags_count + args_count + 1] = NULL;
	return (args);
}

static void	execute_child_process(t_shell *shell, t_command *cmd)
{
	char	*path;
	char	**args;

	args = prepare_args(shell, cmd);
	path = get_path(&shell->gc, args[0]);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	execve(path, args, shell->env);
	perror("execve");
	exit(EXIT_EXECVE_FAILED);
}

int	handle_parent_process(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_command_no_pipes(t_shell *shell, t_command *cmd)
{
	pid_t	pid;

	setup_child_signals();
	pid = fork();
	if (pid == 0)
		execute_child_process(shell, cmd);
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	return (handle_parent_process(pid));
}



static void	fill_args(t_shell *shell, t_command *cmd, char **args, int flags_count, int args_count)
{
	int	j;

	j = 0;
	while (j < args_count)
	{
		args[flags_count + 1 + j] = remove_quotes(&shell->gc, strdup(cmd->args[j]));
		if (!args[flags_count + 1 + j])
			perror_strdup();
		j++;
	}
}

char	**prepare_args(t_shell *shell, t_command *cmd, int flags_count, int args_count)
{
	char	**args;

	args = (char **)ft_gc_malloc(&shell->gc, sizeof(char *) * (flags_count + args_count + 2));
	if (!args)
		perror_malloc();
	args[0] = ft_shell_strdup(&shell->gc, cmd->cmd_name);
	if (!args[0])
		perror_strdup();

	fill_flags(shell, cmd, args, flags_count);
	fill_args(shell, cmd, args, flags_count, args_count);
	args[flags_count + args_count + 1] = NULL;
	return (args);
}
