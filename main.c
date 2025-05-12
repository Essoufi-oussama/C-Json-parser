#include "header.h"

void    error(t_tokens **head)
{
    t_tokens *current;
    t_tokens *next;

    write(2, "Error\n", 7);
    current = *head;
    while(current)
    {
        next = current->next;
        free(current);
        current = next;
    }
    exit(1);
}

void    add_quoted_word(char *line, int *i, t_tokens **head)
{
    (*i)++;
    if (line[*i] == '"')
        (*i)++;
    else
    {
        while(line[*i] && line[*i] != '"')
            (*i)++;
        if (!line[*i])
        {
            free(line);
            error(head);
        }
        (*i)++;
    }
    ft_lstaddback(head, QUOTED_WORD);
}

void    add_unquoted_word(char *line, int *i, t_tokens **head)
{
    int j = *i;

    while(line[*i] && isalpha(line[*i]))
        (*i)++;
    if (*i - j != 4 || *i - j != 5)
        error(head);
    if (strncmp(line + j, "null", 4) == 0 || 
        strncmp(line + j, "false", 5) == 0 ||
        strncmp(line + j, "true", 4) == 0 )
        ft_lstaddback(head, UNQUOTED_WORD);
    else
    {
        free(line);
        error(head);
    }
}

void    add_number(char *line, int *i, t_tokens **head)
{
    int j = *i;

    while(line[*i] && isdigit(line[*i]))
        (*i)++;
    char c = line[*i];
    if (!isprint(c) || line[*i] != ',' || line[*i] != '}' || line[*i] != '[' || line[*i] != ']' || line[*i] != ':')
    {
        free(line);
        error(head);
    }
    ft_lstaddback(head, UNQUOTED_WORD);
}

void	tokenize(t_tokens **head, char *line)
{
    if (line == NULL)
        return ;
    for (int i = 0; line[i]; i++)
    {
        if (line[i] == '"')
            add_quoted_word(line, &i, head);
        else if (isalpha(line[i]))
            add_unquoted_word(line, &i, head);
        else if (isdigit(line[i]))
            add_number(line, &i, head);
    }   
}

t_tokens	*read_file(FILE *fp)
{
	char		buffer[BUFFER_SIZE];
	int			size;
	t_tokens	*head;
	char		*line;
	char		*tmp;

	head = NULL;
	line = NULL;
	tmp = NULL;
	while (!feof(fp))
	{
		line = ft_strdup(fgets(buffer, BUFFER_SIZE, fp));
		if (line == NULL)
			break ;
		if (strchr(line, '\n') == NULL && !feof(fp))
		{
			while (strchr(buffer, '\n') == NULL)
			{
				tmp = line;
				fgets(buffer, BUFFER_SIZE, fp);
				line = strjoin(line, buffer);
				free(tmp);
				if (line == NULL)
					// fatal_error_handle();
				if (feof(fp))
					break ;
			}
		}
		tokenize(&head, line);
	}
	return (head);
}


int	main(int argc, char **argv)
{
	FILE *fp;

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
	read_file(fp);
	return (0);
}