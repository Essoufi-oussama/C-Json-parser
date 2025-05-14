#include "header.h"

int	check_first_and_last(t_tokens **tokens)
{
	int			i;
	t_tokens	*current;

	i = 0;
	current = *tokens;
	if (current == NULL || current->type != OPENING_BRACKET)
		error(tokens);
	while (current->next)
	{
		current = current->next;
		i++;
	}
	if (current->type != CLOSING_BRACKET)
		error(tokens);
	return(i);
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

void	check_array(t_tokens **current, t_tokens **tokens)
{
	*current = (*current)->next;
	while ((*current)->type != CLOSING_ARRAY)
	{
		if ((*current)->type != QUOTED_WORD && (*current)->type != CLOSING_ARRAY)
			error(tokens);
		else
			*current = (*current)->next;
		if ((*current)->type != COMMA && (*current)->type != CLOSING_ARRAY)
			error(tokens);
		if ((*current)->type == COMMA) 
			*current = (*current)->next;
	}
	*current = (*current)->next;
}

void	check_bracket(t_tokens **current, t_tokens **tokens)
{
	if ((*current)->type == OPENING_BRACKET)
		*current = (*current)->next;
	if ((*current)->type == CLOSING_BRACKET)
	{
		*current = (*current)->next;
		return ;
	}
	// check key
	if ((*current)->type != QUOTED_WORD && (*current)->next->type != DOUBLE_POINT)\
		error(tokens);
	else
		*current = (*current)->next->next;
	if ((*current)->type != QUOTED_WORD && (*current)->type != UNQUOTED_WORD && (*current)->type != OPENING_ARRAY && (*current)->type != OPENING_BRACKET)
		error(tokens);
	else if ((*current)->type == OPENING_BRACKET)
		check_bracket(current, tokens);
	else if ((*current)->type == OPENING_ARRAY)
		check_array(current, tokens);
	else
		*current = (*current)->next;
	if ((*current)->type != COMMA && (*current)->type != CLOSING_BRACKET)
		error(tokens);
	if ((*current)->type == COMMA && (*current)->next->type == CLOSING_BRACKET)
		error(tokens);
	(*current) = (*current)->next;
}

void	check_key_value(t_tokens **tokens)
{
	t_tokens	*current;

	current = *tokens;
    while(current)
    {
		check_bracket(&current, tokens);
    }
}

void	lexing(t_tokens **tokens)
{
	check_unopenned_paranth(tokens);
	check_key_value(tokens);
}