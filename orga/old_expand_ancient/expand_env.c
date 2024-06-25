// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   expand_env.c                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: afoth <afoth@student.42berlin.de>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/22 17:25:37 by afoth             #+#    #+#             */
// /*   Updated: 2024/06/21 15:46:24 by afoth            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../include/minishell.h"

// //function to expand environment variables for example $PATH
// // DEL needs testing and implementation of exit_status()

// int	find_end_of_env_in_quotes(char *arg, int i)
// {
// 	int		j;

// 	j = i + 1;
// 	while (arg[j] != '\0')
// 	{
// 		if (arg[j] == ' ' || arg[j] == '\0' || arg[j] == '$' || arg[j] == 34 || arg[j] == 39)
// 			return (j - 2);
// 		j++;
// 	}
// 	return (-1);
// }

// //DEL NOT FINISHED, needs work
// //"a/home/afoth z $USER "
// void handle_expansions_in_quotes(t_arg *head)
// {
// 	int		i;
// 	int		end_of_env;
// 	//char	*temp;
// 	//char	*string;
// 	char	*env;

// 	char	*before_env;
// 	char	*after_env;

// 	i = 0;
// 	end_of_env = 0;
// 	//temp = head->arg;
// 	while (head->arg[i] != '\0')
// 	{
// 		if (head->arg[i] == '$')
// 		{
// 			//printf("i: %d, find_end_of_env_in_quotes: %d\n", i, find_end_of_env_in_quotes(head->arg, i));//DEL
// 			end_of_env = find_end_of_env_in_quotes(head->arg, i);
// 			if (end_of_env == -1)
// 				return ;
// 			ft_printf("end_of_env: %d\n", end_of_env);
// 			ft_printf("i: %d\n", i);
// 			before_env = ft_shell_substr(head->arg, 0, i);
// 			after_env = ft_shell_substr(head->arg, end_of_env + i , ft_strlen(head->arg) - (end_of_env));//DEL len correct?
// 			env = ft_shell_substr(head->arg, i + 1, end_of_env - 1);
// 			// print all variables
// 			ft_printf("before_env: %s|\n", before_env);
// 			ft_printf("env: %s|\n", env);
// 			ft_printf("after_env: %s|\n", after_env);
// 			// printf("temp: %s\n", temp);//DEL
// 			// if (temp == NULL)
// 			// 	memory_error();
// 			head->arg = ft_expand_env(env);
// 			head->arg = ft_shell_strjoin(before_env, head->arg);
// 			head->arg = ft_shell_strjoin(head->arg, after_env);
// 			//ft_printf("string: %s\n", string);
// 			printf("\nhead->arg: %s\n\n", head->arg);//DEL
// 			//temp = NULL;
// 			before_env = NULL;
// 			after_env = NULL;
// 			i = 0;
// 			end_of_env = 0;
// 			env = NULL;
// 		}
// 		i++;
// 	}
// 	// head->arg = string;
// }


// void	handle_expansions(t_arg *head)
// {
// 	char	*temp;

// 	temp = NULL;
// 	if (head == NULL)
// 		return ;
// 	while (head != NULL)
// 	{
// 		if (head->type == ENV_VARIABLE)
// 		{
// 			temp = head->arg;
// 			temp = ft_shell_substr(temp, 1, ft_strlen(temp) - 1);
// 			if (temp == NULL)
// 				memory_error("handle_expansions");
// 			head->arg = ft_expand_env(temp);
// 			printf("head->arg: %s\n", head->arg);//DEL
// 			if (head->arg == 0)
// 			{
// 				return ;
// 			}
// 		}
// 		//NOT FINISHED
// 		else if (head->type == DOUBLE_QUOTED_STRING)
// 		{
// 			handle_expansions_in_quotes(head);
// 		}
// 		head = head->next;
// 	}
// }


// char	*ft_expand_env(char *env)
// {
// 	char	*path;
// 	char	*temp;

// 	//printf("env: %s\n", env);
// 	// if (env == "?")
// 	// 	exit_status(); //DEL needs to be implemented should return the exit status of the last command
// 	temp = getenv(env);
// 	//printf("getenv: %s\n", temp);
// 	if (temp != NULL)
// 	{
// 		path = ft_shell_strdup(temp);
// 		return (path);
// 	}
// 	else
// 	{
// 		printf("The %s environment variable is not set.\n", env);
// 	}
// 	return (0);
// }

// // char *ft_expand_env(char *env)
// // {
// // 	char *temp;

// // 	temp = getenv(env);
// // 	if (temp != NULL)
// // 	{
// // 		return ft_shell_strdup(temp);
// // 	}
// // 	else
// // 	{
// // 		return ft_shell_strdup(""); // Return an empty string if the variable is not set
// // 	}
// // }
// //DEL two definitions of ft_expand_env


// //null protection
// char	*ft_shell_strjoin(char *s1, char *s2)
// {
// 	char	*newstr;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	if (!s1 || !s2)
// 		return (NULL);
// 	newstr = (char *) ft_gc_malloc((ft_strlen(s1)
// 				+ ft_strlen(s2) + 1) * sizeof(char));
// 	if (!newstr)
// 		return (NULL);
// 	while (s1[i])
// 		newstr[j++] = s1[i++];
// 	i = 0;
// 	while (s2[i])
// 		newstr[j++] = s2[i++];
// 	newstr[j] = '\0';
// 	return (newstr);
// }



// char	*ft_shell_substr(const char *s, unsigned int start, size_t len)
// {
// 	char	*ps;
// 	size_t	i;

// 	i = 0;
// 	if (start > ft_strlen(s))
// 		return (ft_shell_strdup(""));
// 	if (len > ft_strlen(s + start))
// 		len = ft_strlen(s + start);
// 	ps = ft_shell_calloc(len + 1, sizeof(char));
// 	if (!ps)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		ps[i] = s[start + i];
// 		i++;
// 	}
// 	return (ps);
// }

// void	*ft_shell_calloc(size_t count, size_t size)
// {
// 	unsigned char	*tmp;
// 	size_t			i;

// 	i = 0;
// 	tmp = ft_gc_malloc(count * size);
// 	if (!tmp)
// 		return (NULL);
// 	while (i < count * size)
// 		tmp[i++] = 0;
// 	return (tmp);
// }
