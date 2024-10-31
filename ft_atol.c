#include "philo.h"

int parse_sign(const char *str, int *i)
{
    int sign;

    sign = 1;
    if (str[*i] == '+' || str[*i] == '-')
    {
        if (str[*i] == '-')
            sign = -1;
        (*i)++;
    }
    return (sign);
}

int skip_whitespace(const char *str, int *i)
{
    while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
        (*i)++;
    return (0);
}

int is_numeric_string(const char *str, int i)
{
    if (str[i] < '0' || str[i] > '9')
        return (0);
    return (1);
}

long ft_atol(const char *str, int *error)
{
    int     i;
    long    result;
    int     sign;

    i = 0;
    result = 0;
    *error = 0;
    skip_whitespace(str, &i);
    sign = parse_sign(str, &i);
    if (!is_numeric_string(str, i))
    {
        *error = 1;
        return (0);
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    if (str[i] != '\0')
        *error = 1;
    return (sign * result);
}