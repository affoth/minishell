// int ft_input_check(char *line)
// {
//     int i;
//     int single_quotes;
//     int double_quotes;

//     i = 0;
//     single_quotes = 0;
//     double_quotes = 0;

//     while (line[i] != '\0')
// 	{
//         if (line[i] == '\'')
//             single_quotes++;
//         if (line[i] == '"')
//             double_quotes++;
//         i++;
//     }
//     if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
// 	{
//         perror("Quotations not closed");
//         return (1);
//     }
//     return (0);
// }

static size_t	ft_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}
