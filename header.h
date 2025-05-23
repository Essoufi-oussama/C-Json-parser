#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 50

typedef struct s_tokens
{
	int				type;
	int				number;
	struct s_tokens	*next;
}					t_tokens;

typedef enum types
{
	QUOTED_WORD,
	UNQUOTED_WORD,
	DOUBLE_POINT,
	COMMA,
	OPENING_ARRAY = '[',
	CLOSING_ARRAY = ']',
	OPENING_BRACKET = '{',
	CLOSING_BRACKET = '}',
}					type;

char				*strjoin(char *s1, char *s2);
char				*ft_strdup(const char *s1);
void				ft_lstaddback(t_tokens **head, int type, FILE *fp);
void				lstclear(t_tokens **head);
void				check_empty_file(FILE *fp);
void				fatal_error_handle(t_tokens **head);
void				check_unopenned_paranth(t_tokens **tokens);
void				error(t_tokens **head);
void				lexing(t_tokens **tokens);
void		check_bracket(t_tokens **current, t_tokens **tokens);
void	error_in_parsing(FILE *fp, char *line, t_tokens **head);

