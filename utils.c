#include "header.h"

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*s2;

	i = strlen(s1);
	s2 = (char *)malloc(i + 1);
	if (s2 == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

char    *strjoin(char *s1, char *s2)
{
    char	*str;
	size_t	i;
	size_t	j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	str = (char *) malloc(strlen(s1) + strlen(s2) + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

void	check_empty_file(FILE *fp)
{
	int	c;

	c = fgetc(fp);
	if (c == EOF)
	{
		fprintf(stderr, "error : invalid JSON\n");
		exit(1);
	}
	else
		ungetc(c, fp);
}

void	lstclear(t_tokens **head)
{
	t_tokens	*current;
	t_tokens	*tmp;

	if (head == NULL)
		return ;
	current = *head;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
}

void	ft_lstaddback(t_tokens **head, int type)
{
	t_tokens	*new;
    t_tokens    *current;

	new = malloc(sizeof(t_tokens));
	if (new == NULL)
	{
		lstclear(head);
	}
    new->type = type;
    current = *head;
    if (current == NULL)
    {
        *head = new;
        return ;
    }
    while (current->next)
        current = current->next;
    current->next = new;
}