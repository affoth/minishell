/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:18:40 by mokutucu          #+#    #+#             */
/*   Updated: 2024/06/03 14:20:39 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//added by afoth
int	find_end_of_env_in_quotes(char *arg, int i)
{
	int		j;

	j = i + 1;
	while (arg[j] != '\0')
	{
		if (arg[j] == ' ' || arg[j] == '\0' || arg[j] == '$' || arg[j] == 34)
			return (j - 2);
		j++;
	}
	return (-1);
}

//DEL NOT FINISHED, needs work
void handle_expansions_in_quotes(t_arg *head)
{
	int		i;
	char	*temp;

	temp = NULL;
	i = 0;
	temp = head->arg;
	while (head->arg[i] != '\0')
	{
		if (head->arg[i] == '$')
		{
			printf("i: %d, find_end_of_env_in_quotes: %d\n", i, find_end_of_env_in_quotes(head->arg, i));//DEL
			temp = ft_substr(temp, i + 1, find_end_of_env_in_quotes(head->arg, i));
			printf("temp: %s\n", temp);//DEL
			if (temp == NULL)
				memory_error();
			head->arg = ft_expand_env(temp);
			printf("head->arg: %s\n", head->arg);//DEL
			if (head->arg == 0)
			{
				free(temp);
				return ;
			}
			free(temp);
		}
		i++;
	}
}


void	handle_expansions(t_arg *head)
{
	char	*temp;

	temp = NULL;
	if (head == NULL)
		return ;
	while (head != NULL)
	{
		if (head->type == ENV_VARIABLE)
		{
			temp = head->arg;
			temp = ft_substr(temp, 1, ft_strlen(temp) - 1);
			if (temp == NULL)
				memory_error();
			head->arg = ft_expand_env(temp);
			printf("head->arg: %s\n", head->arg);//DEL
			if (head->arg == 0)
			{
				free(temp);
				return ;
			}
			free(temp);
		}
		//NOT FINISHED
		else if (head->type == DOUBLE_QUOTED_STRING)
		{
			handle_expansions_in_quotes(head);
		}
		head = head->next;
	}
}


char	*ft_expand_env(char *env)
{
	char	*path;
	char	*temp;

	//printf("env: %s\n", env);
	// if (env == "?")
	// 	exit_status(); //DEL needs to be implemented should return the exit status of the last command
	temp = getenv(env);
	//printf("getenv: %s\n", temp);
	if (temp != NULL)
	{
		path = ft_shell_strdup(temp);
		return (path);
	}
	else
	{
		printf("The %s environment variable is not set.\n", env);
	}
	return (0);
}
