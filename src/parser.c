/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokutucu <mokutucu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:08:45 by mokutucu          #+#    #+#             */
/*   Updated: 2024/05/22 17:56:23 by mokutucu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

typedef struct s_arg
{
	char *arg;
	struct s_arg *prev;
	struct s_arg *next;
	bool is_double_quoted;
} t_arg;

int ft_input_check(char *line)
{
	int i;
	int single_quotes;
	int double_quotes;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;

	while (line[i] != '\0') {
		if (line[i] == '\'')
			single_quotes++;
		if (line[i] == '"')
			double_quotes++;
		i++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0) {
		perror("Quotations not closed");
		return (1);
	}
	return (0);
}

//populate the arguments into the list, could also consider dummy node as head with no arg
void put_args(t_arg *args, char **split_args)
{
	int i;

	i = 0;

	while (split_args[i] != NULL)
	{
		//check is double quoted
		if (split_args[i][0] == '"' || split_args[i][ft_strlen(split_args[i]) - 1] == '"') {
			args[i].is_double_quoted = true;
		} else {
			args[i].is_double_quoted = false;
		}
		args[i].arg = strdup(split_args[i]);
		if (i > 0) {
			args[i].prev = &args[i - 1];
			args[i - 1].next = &args[i];
		} else {
			args[i].prev = NULL;
		}
		i++;
	}
	args[i].arg = NULL;
	args[i].next = NULL;
}

t_arg *create_list_of_args(char *line)
{
	t_arg *args;
	char **split_args;
	int count;
	int i;
	count = 0;
	i = 0;

	split_args = ft_split(line, ' ');
	while (split_args[count] != NULL)
		count++;

	//allocate memory for each argument
	args = (t_arg *)malloc(sizeof(t_arg) * (count + 1));
	if (!args) {
		perror("Malloc failed");
		exit(1);
	}
	put_args(args, split_args);

	while (split_args[i] != NULL)
	{
		free(split_args[i]);
		i++;
	}
	free(split_args);

	return args;
}

//test list
void print_args(t_arg *args) {
	int i;
	bool is_double_quoted = false; // or true, depending on your needs
	i = 0;
	while (args[i].arg != NULL)
	{
		printf("args[%d]: %s, next: %p, prev: %p\n, double_quote: %s \n", i, args[i].arg, (void *)args[i].next, (void *)args[i].prev, is_double_quoted ? "true" : "false");
		i++;
	}
}

void free_args(t_arg *args) {
	int i = 0;
	while (args[i].arg != NULL) {
		free(args[i].arg);
		i++;
	}
	free(args);
}

void parser(char *line) {
	t_arg *args;

	args = create_list_of_args(line);
	print_args(args);
	free_args(args);
}
