#include "philo.h"

int allocate_resources(pthread_t **threads, pthread_mutex_t **forks,
                       t_philosopher **philos, t_params *params)
{
    *threads = malloc(sizeof(pthread_t) * params->num_philosophers);
    *forks = malloc(sizeof(pthread_mutex_t) * params->num_philosophers);
    *philos = malloc(sizeof(t_philosopher) * params->num_philosophers);
    if (!*threads || !*forks || !*philos)
        return (1);
    return (0);
}

void init_mutexes(pthread_mutex_t *forks, t_params *params)
{
    int i;

    pthread_mutex_init(&params->stop_mutex, NULL);
    i = 0;
    while (i < params->num_philosophers)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
}

void init_philosophers(t_philosopher *philos, pthread_mutex_t *forks, t_params *params)
{
    int i;
    int num;

    i = 0;
    num = params->num_philosophers;
    while (i < num)
    {
        philos[i].id = i + 1;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % num];
        philos[i].params = params;
        philos[i].meals_eaten = 0;
        i++;
    }
}

void create_philosopher_threads(pthread_t *threads, t_philosopher *philos, int num_philosophers)
{
    int i;

    i = 0;
    while (i < num_philosophers)
    {
        pthread_create(&threads[i], NULL, philosopher_routine, &philos[i]);
        i++;
    }
}
