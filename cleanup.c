#include "philo.h"

void join_threads(pthread_t *threads, int num_philosophers)
{
    int i;

    i = 0;
    while (i < num_philosophers)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
}

void destroy_mutexes(pthread_mutex_t *forks, t_params *params)
{
    int i;

    i = 0;
    while (i < params->num_philosophers)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_mutex_destroy(&params->stop_mutex);
}

void free_resources(pthread_t *threads, pthread_mutex_t *forks, t_philosopher *philos)
{
    free(threads);
    free(forks);
    free(philos);
}
