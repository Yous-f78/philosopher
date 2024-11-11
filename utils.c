#include "philo.h"

/// @brief Returns the current timestamp in milliseconds.
/// @return The current time in milliseconds.
long int timestamp(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/// @brief Sleeps for the specified number of milliseconds.
/// @param ms The number of milliseconds to sleep.
/// @return Returns 0 on success, or -1 on error.
int sleep_ms(int ms)
{
    return (usleep(ms * 1000));
}

/// @brief Prints the philosopher's action in French.
/// @param philosopher Pointer to the philosopher structure.
/// @param action Character representing the action ('f', 'e', 's', 'd', 't').
/// @param elapsed_time Time elapsed since the simulation started.
static void print_action_in_french(t_philosopher *philosopher, char action, long elapsed_time)
{
    int id = philosopher->id;

    if (action == 'f')
        printf("%s%ld %d a pris une fourchette\n%s", GREEN, elapsed_time, id, RESET);
    else if (action == 'e')
        printf("%s%ld %d mange\n%s", ORANGE, elapsed_time, id, RESET);
    else if (action == 's')
        printf("%s%ld %d dort\n%s", CYAN, elapsed_time, id, RESET);
    else if (action == 'd')
        printf("%s%ld %d est mort\n%s", RED, elapsed_time, id, RESET);
    else if (action == 't')
        printf("%s%ld %d réfléchit...\n%s", WHITE, elapsed_time, id, RESET);
}

/// @brief Prints the philosopher's action in English or French based on the FRENCH macro.
/// @param philosopher Pointer to the philosopher structure.
/// @param action Character representing the action ('f', 'e', 's', 'd', 't').
void print_action(t_philosopher *philosopher, char action)
{
    int id = philosopher->id;
    long elapsed_time = timestamp() - philosopher->simulation->start_time;

    // Lock the print mutex to prevent concurrent prints
    pthread_mutex_lock(&philosopher->simulation->print_mutex);

    if (FRENCH)
    {
        print_action_in_french(philosopher, action, elapsed_time);
    }
    else
    {
        if (action == 'f')
            printf("%s%ld %d has taken a fork\n%s", GREEN, elapsed_time, id, RESET);
        else if (action == 'e')
            printf("%s%ld %d is eating\n%s", ORANGE, elapsed_time, id, RESET);
        else if (action == 's')
            printf("%s%ld %d is sleeping\n%s", CYAN, elapsed_time, id, RESET);
        else if (action == 'd')
            printf("%s%ld %d died\n%s", RED, elapsed_time, id, RESET);
        else if (action == 't')
            printf("%s%ld %d is thinking\n%s", WHITE, elapsed_time, id, RESET);
    }

    // Unlock the print mutex
    pthread_mutex_unlock(&philosopher->simulation->print_mutex);
}

/// @brief Converts the initial portion of the string pointed to by str to an integer.
/// @param str The string to convert.
/// @return The converted integer value.
int ft_atoi(const char *str)
{
    int number = 0;
    int index = 0;
    int sign = 1;

    // Skip whitespace characters
    while ((9 <= str[index] && str[index] <= 13) || str[index] == 32)
        index++;

    // Check for sign indicators
    if (str[index] == '+' || str[index] == '-')
    {
        if (str[index] == '-')
            sign = -1;
        index++;
    }

    // Convert numeric characters to integer
    while ('0' <= str[index] && str[index] <= '9')
    {
        number = number * 10 + (str[index] - '0');
        index++;
    }

    return (number * sign);
}
