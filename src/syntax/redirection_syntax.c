#include "../../include/minishell.h"

// Function to check if a character is valid in a filename
int is_valid_char(char c)
{
	if (ft_isalnum(c) || c == '.' || c == '-' || c == '_')
	{
		return 1;
	}
	return 0;
}

// Function to check if a filename is valid
int is_valid_filename(const char *filename)
{
	if (filename == NULL || filename[0] == '\0')
	{
		return 0;
	}

	// Check each character in the filename
	for (size_t i = 0; i < ft_strlen(filename); ++i)
	{
		if (!is_valid_char(filename[i]))
		{
			return 0;
		}
	}

	return 1;
}

// check for syntax errors in redirections
int	redirection_syntax(t_arg *head)
{
	t_arg *tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == REDIRECTION_OUT || tmp->type == REDIRECTION_IN || tmp->type == REDIRECTION_APPEND || tmp->type == HEREDOC)
		{
			// check for no arguments before or after redirection
			if (!tmp->next || !tmp->prev)
			{
				ft_printf("redirection error: no arguments before or after redirection `%s'\n", tmp->arg);
				return (1);
			}
			// check for valid file name
			if (tmp->next->type != WORD)
			{
				ft_printf("redirection error: no valid file name after redirection `%s'\n", tmp->arg);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}