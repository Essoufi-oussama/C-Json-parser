#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define BUFFER_SIZE 50

typedef struct s_tokens
{
    int     type;
    struct  s_tokens *next;
} t_tokens;

typedef enum types
{
    LEFT_BRACKET,
    RIGHT_BRACKET,
    QUOTED_WORD,
    UNQUOTED_WORD,
    DOUBLE_POINT,
    COMMA,
    OPENING_ARRAY,
    CLOSING_ARRAY,
    OPNING_BRACKET,
    CLOSING_BRACKET,
} type;

char    *strjoin(char *s1, char *s2);
char	*ft_strdup(const char *s1);
void	ft_lstaddback(t_tokens **head, int type, FILE *fp);
void	lstclear(t_tokens **head);
void	check_empty_file(FILE *fp);
void    fatal_error_handle(t_tokens **head);
