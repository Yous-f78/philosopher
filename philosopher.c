#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher   *philo = (t_philosopher *)arg;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    set_fork_order(philo, &first_fork, &second_fork);
    gettimeofday(&philo->last_meal_time, NULL);
    while (1)
    {
        if (check_death(philo))
            break;
        print_status(philo, "is thinking");
        take_forks(philo, first_fork, second_fork);
        eat(philo);
        put_down_forks(first_fork, second_fork);
        print_status(philo, "is sleeping");
        precise_sleep(philo->params->time_to_sleep);
    }
    return NULL;
}

int check_death(t_philosopher *philo)
{
    struct timeval current_time;
    long time_since_last_meal;

    pthread_mutex_lock(&philo->params->stop_mutex);
    if (philo->params->stop)
    {
        pthread_mutex_unlock(&philo->params->stop_mutex);
        return 1;
    }
    pthread_mutex_unlock(&philo->params->stop_mutex);
    gettimeofday(&current_time, NULL);
    time_since_last_meal = (current_time.tv_sec - philo->last_meal_time.tv_sec) * 1000 +
                           (current_time.tv_usec - philo->last_meal_time.tv_usec) / 1000;
    if (time_since_last_meal >= philo->params->time_to_die)
    {
        pthread_mutex_lock(&philo->params->stop_mutex);
        philo->params->stop = 1;
        pthread_mutex_unlock(&philo->params->stop_mutex);
        printf("%ld %d died\n", get_timestamp(philo->params->start_time), philo->id);
        return 1;
    }
    return 0;
}

void take_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_lock(first_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(second_fork);
    print_status(philo, "has taken a fork");
}

void eat(t_philosopher *philo)
{
    gettimeofday(&philo->last_meal_time, NULL);
    print_status(philo, "is eating");
    precise_sleep(philo->params->time_to_eat);
}

void put_down_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}
