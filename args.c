#include "philo.h"

int print_usage(void)
{
    printf("Usage: ./philo number_of_philosophers time_to_die ");
    printf("time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
    return (1);
}

int print_error(void)
{
    printf("All arguments must be positive integers\n");
    return (1);
}

long parse_positive_long(const char *str, int *error_flag)
{
    long value;

    value = ft_atol(str, error_flag);
    if (*error_flag || value <= 0)
    {
        *error_flag = 1;
        return (0);
    }
    return (value);
}

int parse_arguments(int argc, char **argv, t_params *params)
{
    int error;

    if (argc != 5 && argc != 6)
        return (print_usage());
    error = 0;
    params->num_philosophers = (int)parse_positive_long(argv[1], &error);
    params->time_to_die = parse_positive_long(argv[2], &error);
    params->time_to_eat = parse_positive_long(argv[3], &error);
    params->time_to_sleep = parse_positive_long(argv[4], &error);
    if (argc == 6)
        params->times_must_eat = (int)parse_positive_long(argv[5], &error);
    else
        params->times_must_eat = -1;
    if (error)
        return (print_error());
    return (0);
}
