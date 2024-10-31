#include "philo.h"

void set_fork_order(t_philosopher *philo, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork)
{
    if (philo->id % 2 == 0)
    {
        *first_fork = philo->right_fork;
        *second_fork = philo->left_fork;
    }
    else
    {
        *first_fork = philo->left_fork;
        *second_fork = philo->right_fork;
    }
}

void print_status(t_philosopher *philo, char *message)
{
    pthread_mutex_lock(&philo->params->stop_mutex);
    if (philo->params->stop)
    {
        pthread_mutex_unlock(&philo->params->stop_mutex);
        return;
    }
    printf("%ld %d %s\n", get_timestamp(philo->params->start_time), philo->id, message);
    pthread_mutex_unlock(&philo->params->stop_mutex);
}

long get_timestamp(struct timeval start_time)
{
    struct timeval current_time;
    long seconds, microseconds;

    gettimeofday(&current_time, NULL);
    seconds = current_time.tv_sec - start_time.tv_sec;
    microseconds = current_time.tv_usec - start_time.tv_usec;
    return (seconds * 1000) + (microseconds / 1000);
}

void precise_sleep(t_philosopher *philo, long sleep_time_ms)
{
    struct timeval start_time, current_time;
    long elapsed_time_ms;

    gettimeofday(&start_time, NULL);
    elapsed_time_ms = 0;
    while (elapsed_time_ms < sleep_time_ms)
    {
        usleep(100);
        if (check_stop(philo))
            break;
        gettimeofday(&current_time, NULL);
        elapsed_time_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 +
                          (current_time.tv_usec - start_time.tv_usec) / 1000;
    }
}

int check_stop(t_philosopher *philo)
{
    int stop;

    pthread_mutex_lock(&philo->params->stop_mutex);
    stop = philo->params->stop;
    pthread_mutex_unlock(&philo->params->stop_mutex);
    return stop;
}
