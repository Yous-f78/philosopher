#include "philo.h"

void take_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    if (check_stop(philo))
        return;
    pthread_mutex_lock(first_fork);
    if (check_stop(philo))
    {
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(second_fork);
    if (check_stop(philo))
    {
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");
}

void eat(t_philosopher *philo)
{
    if (check_stop(philo))
        return;
    gettimeofday(&philo->last_meal_time, NULL);
    print_status(philo, "is eating");
    precise_sleep(philo, philo->params->time_to_eat);
}

void put_down_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}