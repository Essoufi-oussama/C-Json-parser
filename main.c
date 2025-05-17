#include "header.h"

void	check_hex_value(char *line, int i, t_tokens **head, FILE *fp)
{
	if (!line[i])
		error_in_parsing(fp, line, head);
	if ((line[i] >= 'A' && line[i] <= 'F' )|| (line[i] >= 'a' && line[i] <= 'f') || (line[i] >= '0' && line[i] <= '9'))
		return ;
	error_in_parsing(fp, line, head);
}

void	escape_handle(char *line, int *i, t_tokens **head, FILE *fp)
{
	(*i)++;
	if(line[*i] == 'u')
	{
		(*i)++;
		for (int j = 0; j < 4; j++)
			check_hex_value(line, *i + j, head, fp);
		*i = *i + 4;
	}
	else if (line[*i] != '\"' && line[*i] != '\\' && line[*i] != '/' && line[*i] != 'b' && line[*i] != 'f' && line[*i] != 'n' && line[*i] != 'r' && line[*i] != 't')
		error_in_parsing(fp, line, head);
}

void	add_quoted_word(char *line, int *i, t_tokens **head, FILE *fp)
{
	(*i)++;
	ft_lstaddback(head, QUOTED_WORD, fp);
	if (line[*i] == '"')
		return ;
	else
	{
		while (line[*i] && line[*i] != '"')
		{
			// check_escape_values
			if (line[*i] =='\t' || line[*i] =='\n')
				error_in_parsing(fp, line, head);
			if (line[*i] =='\\')
				escape_handle(line, i, head, fp);
			(*i)++;
		}
		if (!line[*i])
			error_in_parsing(fp, line, head);
	}
}

void	add_unquoted_word(char *line, int *i, t_tokens **head, FILE *fp)
{
	int	j;

	j = *i;
	while (line[*i] && isalpha(line[*i]))
		(*i)++;
	// only null false or true are allowed in unquoted word
	if ((strncmp(line + j, "null", 4) == 0|| strncmp(line + j, "true", 4) == 0) && *i - j == 4)
		ft_lstaddback(head, UNQUOTED_WORD, fp);
	else if (strncmp(line + j, "false", 5) == 0 && *i - j == 5)
		ft_lstaddback(head, UNQUOTED_WORD, fp);
	else
		error_in_parsing(fp, line, head);
	(*i)--;
}

void	check_exponential(int *e_count, int *i, char *line, FILE *fp, t_tokens **head)
{
	(*e_count)++;
	(*i)++;
	if (line[*i] == '+' || line[*i] == '-')
		(*i)++;
	if (*e_count > 1 || !isdigit(line[*i]))
		error_in_parsing(fp, line, head);
}

void	add_number(char *line, int *i, t_tokens **head, FILE *fp)
{
	int		e_count = 0;
	int		float_count = 0;
	//numbers cant have leading zero
	if (line[*i] == '0'  && isdigit(line[*i + 1]))
		error_in_parsing(fp, line, head);
	if (line[*i] == '-')
		(*i)++;
	while (line[*i] && isdigit(line[*i]))
	{
		(*i)++;
		if (line[*i] == 'e' || line[*i] == 'E')
			check_exponential(&e_count, i, line , fp, head);
		else if (line[*i] == '.')
		{
			float_count++;
			(*i)++;
			if (float_count > 1)
				error_in_parsing(fp, line, head);
		}
	}
	char c = line[*i];
	if (isprint(c) && (c != ',' && c != '}' && c != '{' && c != '[' && c != ']'
		&& c != ':' && c != ' '))
		error_in_parsing(fp, line, head);
	ft_lstaddback(head, UNQUOTED_WORD, fp);
	(*i)--;
}

void	tokenize(t_tokens **head, char *line, FILE *fp)
{
	int	i;

	i = 0;
	if (line == NULL)
		return ;
	while (line[i])
	{
		if (line[i] == '"')
			add_quoted_word(line, &i, head, fp);
		else if (isalpha(line[i]))
			add_unquoted_word(line, &i, head, fp);
		else if (isdigit(line[i]) || line[i] == '-')
			add_number(line, &i, head, fp);
		else if (line[i] == '{')
			ft_lstaddback(head, OPENING_BRACKET, fp);
		else if (line[i] == '}')
			ft_lstaddback(head, CLOSING_BRACKET, fp);
		else if (line[i] == '[')
			ft_lstaddback(head, OPENING_ARRAY, fp);
		else if (line[i] == ']')
			ft_lstaddback(head, CLOSING_ARRAY, fp);
		else if (line[i] == ',')
			ft_lstaddback(head, COMMA, fp);
		else if (line[i] == ':')
			ft_lstaddback(head, DOUBLE_POINT, fp);
		else if (!isprint(line[i]) || line[i] == ' ')
		{
			i++;
			continue ;
		}
		else
			error_in_parsing(fp, line, head);
		i++;
	}
}

t_tokens	*read_file(FILE *fp)
{
	char		buffer[BUFFER_SIZE];
	t_tokens	*head;
	char		*line;
	char		*tmp;

	head = NULL;
	line = NULL;
	while (fgets(buffer, BUFFER_SIZE, fp))
	{
		tmp = ft_strdup(buffer);
		if (!tmp)
			fatal_error_handle(&head);
		while (!strchr(tmp, '\n') && !feof(fp))
		{
			if (!fgets(buffer, BUFFER_SIZE, fp))
				break ;
			line = strjoin(tmp, buffer);
			free(tmp);
			if (!line)
			{
				fclose(fp);
				fatal_error_handle(&head);
			}
			tmp = line;
		}
		tokenize(&head, tmp, fp);
		free(tmp);
	}
	return (head);
}

int	main(int argc, char **argv)
{
	FILE *fp;
	t_tokens *tokens;

	if (argc != 2)
	{
		fprintf(stderr, "error: usage ./C_JSON_PARSER file.json");
		return (1);
	}
	fp = fopen(argv[1], "r");
	if (!fp)
	{
		fprintf(stderr, "error opening file %s\n", argv[1]);
		return (1);
	}
	check_empty_file(fp);
	tokens = read_file(fp);
	fclose(fp);
	lexing(&tokens);
	lstclear(&tokens);
	return (0);
}
