#include "philo.h"

long ft_atoi(const char *str)
{
    int     i;
    long    result;
    int     sign;

    i = 0;
    result = 0;
    sign = 1;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (sign * result);
}

int parse_arguments(int argc, char **argv, t_params *params)
{
    if (argc != 5 && argc != 6)
    {
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }
    params->num_philosophers = ft_atoi(argv[1]);
    params->time_to_die = atol(argv[2]);
    params->time_to_eat = atol(argv[3]);
    params->time_to_sleep = atol(argv[4]);
    if (argc == 6)
        params->times_must_eat = atoi(argv[5]);
    else
        params->times_must_eat = -1;
    if (params->num_philosophers <= 0 || params->time_to_die <= 0 ||
        params->time_to_eat <= 0 || params->time_to_sleep <= 0 ||
        (argc == 6 && params->times_must_eat <= 0))
    {
        printf("All arguments must be positive integers\n");
        return (1);
    }
    return (0);
}
