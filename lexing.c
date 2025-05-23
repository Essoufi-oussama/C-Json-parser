#include "header.h"

int	check_first_and_last(t_tokens **tokens)
{
	int			i;
	t_tokens	*current;
	int			first;

	i = 0;
	current = *tokens;
	first = current->type;
	if (current == NULL || (current->type != OPENING_BRACKET
			&& current->type != OPENING_ARRAY))
		error(tokens);
	while (current->next)
	{
		current = current->next;
		i++;
	}
	if (current->type != CLOSING_BRACKET && current->type != CLOSING_ARRAY)
		error(tokens);
	if ((first == OPENING_BRACKET && current->type != CLOSING_BRACKET)
		|| (first == OPENING_ARRAY && current->type != CLOSING_ARRAY))
		error(tokens);
	return (i);
}

void	check_unopenned_paranth(t_tokens **tokens)
{
	int			top;
	int			*stack;
	t_tokens	*current;
	int			i;

	top = -1;
	current = *tokens;
	i = check_first_and_last(tokens);
	stack = malloc(sizeof(int) * (i + 1));
	if (stack == NULL)
		fatal_error_handle(tokens);
	current = *tokens;
	while (current)
	{
		if (current->type == '[' || current->type == ']' || current->type == '{'
			|| current->type == '}')
		{
			if (current->type == '[' || current->type == '{')
				stack[++top] = current->type;
			else
			{
				if (top == -1)
				{
					free(stack);
					error(tokens);
				}
				if ((current->type == '}' && stack[top] != '{')
					|| (current->type == ']' && stack[top] != '['))
				{
					free(stack);
					error(tokens);
				}
				top--;
			}
		}
		current = current->next;
	}
	free(stack);
	if (top != -1)
		error(tokens);
}

void	check_array(t_tokens **current, t_tokens **tokens, int i)
{
	*current = (*current)->next;
	if ((*current)->type == OPENING_ARRAY)
		check_array(current, tokens, i + 1);
	if (i >= 20)
		error(tokens);
	while ((*current)->type != CLOSING_ARRAY)
	{
		if ((*current)->type == OPENING_BRACKET)
			check_bracket(current, tokens);
		else if ((*current)->type == OPENING_ARRAY)
			check_array(current, tokens, 1);
		else
			*current = (*current)->next;
		if ((*current)->type != COMMA && (*current)->type != CLOSING_ARRAY)
			error(tokens);
		if ((*current)->type == COMMA && (*current)->next->type != CLOSING_ARRAY)
			*current = (*current)->next;
	}
	*current = (*current)->next;
}

void	check_key(t_tokens **current, t_tokens **tokens)
{
	if ((*current)->type != QUOTED_WORD)
		error(tokens);
	*current = (*current)->next;
	if ((*current)->type != DOUBLE_POINT)
		error(tokens);
	*current = (*current)->next;
}

void	check_value(t_tokens **current, t_tokens **tokens)
{
	if ((*current)->type != QUOTED_WORD && (*current)->type != UNQUOTED_WORD
		&& (*current)->type != OPENING_ARRAY
		&& (*current)->type != OPENING_BRACKET)
		error(tokens);
	else if ((*current)->type == OPENING_BRACKET)
		check_bracket(current, tokens);
	else if ((*current)->type == OPENING_ARRAY)
		check_array(current, tokens, 1);
	else
		*current = (*current)->next;
	if ((*current)->type != COMMA && (*current)->type != CLOSING_BRACKET)
		error(tokens);
	if ((*current)->type == COMMA && (*current)->next->type == CLOSING_BRACKET)
		error(tokens);
}

void	check_bracket(t_tokens **current, t_tokens **tokens)
{
	*current = (*current)->next;
	while (*current && (*current)->type != CLOSING_BRACKET)
	{
		check_key(current, tokens);
		check_value(current, tokens);
		if ((*current)->type != CLOSING_BRACKET)
			(*current) = (*current)->next;
	}
	*current = (*current)->next;
}

void	check_tokens(t_tokens **tokens)
{
	t_tokens	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == OPENING_ARRAY)
			check_array(&current, tokens, 1);
		else
			check_bracket(&current, tokens);
	}
}

void	lexing(t_tokens **tokens)
{
	check_unopenned_paranth(tokens);
	check_tokens(tokens);
}