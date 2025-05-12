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
    LEFT_ARRAY,
    RIGHT_ARRAY,
    LEFT_BRACE,
    RIGHT_BRACE,
    UNSUPPORTED,

} type;

char    *strjoin(char *s1, char *s2);
char	*ft_strdup(const char *s1);
void	ft_lstaddback(t_tokens **head, int type);
void	lstclear(t_tokens **head);
void	check_empty_file(FILE *fp);
