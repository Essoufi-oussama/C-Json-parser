#include "header.h"

void	error(t_tokens **head)
{
	t_tokens	*current;
	t_tokens	*next;

	write(2, "Error: Invalid JSON\n", 21);
	current = *head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	exit(1);
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
			(*i)++;
		if (!line[*i])
		{
			fclose(fp);
			free(line);
			error(head);
		}
	}
}

void	add_unquoted_word(char *line, int *i, t_tokens **head, FILE *fp)
{
	int	j;

	j = *i;
	while (line[*i] && isalpha(line[*i]))
		(*i)++;
	if (*i - j < 4 || *i - j > 5)
	{
		fclose(fp);
		free(line);
		error(head);
	}
	if (strncmp(line + j, "null", 4) == 0 || strncmp(line + j, "false", 5) == 0
		|| strncmp(line + j, "true", 4) == 0)
		ft_lstaddback(head, UNQUOTED_WORD, fp);
	else
	{
		fclose(fp);
		free(line);
		error(head);
	}
	(*i)--;
}

void	add_number(char *line, int *i, t_tokens **head, FILE *fp)
{
	int		j;
	char	c;

	j = *i;
	while (line[*i] && isdigit(line[*i]))
		(*i)++;
	c = line[*i];
	if (isprint(c) && (c != ',' && c != '}' && c != '{' && c != '[' && c != ']'
			&& c != ':'))
	{
		write(2, &c, 1);
		fclose(fp);
		free(line);
		error(head);
	}
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
		else if (isdigit(line[i]))
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
		else if (!isprint(line[i]) || line[i] == 32)
		{
			i++;
			continue ;
		}
		else
		{
			free(line);
			fclose(fp);
			error(head);
		}
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

	if (argc > 1)
		fp = fopen(argv[1], "r");
	else
		fp = stdin;
	if (!fp)
	{
		fprintf(stderr, "error opening file %s\n", argv[1]);
		return (1);
	}
	check_empty_file(fp);
	tokens = read_file(fp);
	lexing(&tokens);
	lstclear(&tokens);
	fclose(fp);
	return (0);
}