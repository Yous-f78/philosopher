#include "philo.h"

void init_mutexes(pthread_mutex_t *fork1, pthread_mutex_t *fork2, t_params *params)
{
    pthread_mutex_init(fork1, NULL);
    pthread_mutex_init(fork2, NULL);
    pthread_mutex_init(&params->stop_mutex, NULL);
}

void init_params(t_params *params)
{
    params->time_to_die = 500;   // 2000 milliseconds
    params->time_to_eat = 200;    // 500 milliseconds
    params->time_to_sleep = 200;  // 500 milliseconds
    gettimeofday(&params->start_time, NULL);
    params->stop = 0;
}

void init_philosophers(t_philosopher *philo1, t_philosopher *philo2,
                       pthread_mutex_t *fork1, pthread_mutex_t *fork2, t_params *params)
{
    philo1->id = 1;
    philo1->left_fork = fork1;
    philo1->right_fork = fork2;
    philo1->params = params;

    philo2->id = 2;
    philo2->left_fork = fork2;
    philo2->right_fork = fork1;
    philo2->params = params;
}

void create_philosopher_threads(pthread_t *philo_thread1, pthread_t *philo_thread2,
                                t_philosopher *philo1, t_philosopher *philo2)
{
    pthread_create(philo_thread1, NULL, philosopher_routine, philo1);
    pthread_create(philo_thread2, NULL, philosopher_routine, philo2);
}

int main()
{
    pthread_t       philo_thread1;
    pthread_t       philo_thread2;
    pthread_mutex_t fork1;
    pthread_mutex_t fork2;
    t_philosopher   philo1;
    t_philosopher   philo2;
    t_params        params;

    init_mutexes(&fork1, &fork2, &params);
    init_params(&params);
    init_philosophers(&philo1, &philo2, &fork1, &fork2, &params);
    create_philosopher_threads(&philo_thread1, &philo_thread2, &philo1, &philo2);

    pthread_join(philo_thread1, NULL);
    pthread_join(philo_thread2, NULL);

    pthread_mutex_destroy(&fork1);
    pthread_mutex_destroy(&fork2);
    pthread_mutex_destroy(&params.stop_mutex);

    return 0;
}
