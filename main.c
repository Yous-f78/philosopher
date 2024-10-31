#include "philo.h"

void init_params(t_params *params)
{
    gettimeofday(&params->start_time, NULL);
    params->stop = 0;
    params->finished_philosophers = 0;
}

int main(int argc, char **argv)
{
    t_params        params;
    pthread_t       *threads;
    pthread_mutex_t *forks;
    t_philosopher   *philos;

    if (parse_arguments(argc, argv, &params))
        return (1);
    init_params(&params);
    if (allocate_resources(&threads, &forks, &philos, &params))
        return (1);
    init_mutexes(forks, &params);
    init_philosophers(philos, forks, &params);
    create_philosopher_threads(threads, philos, params.num_philosophers);
    join_threads(threads, params.num_philosophers);
    destroy_mutexes(forks, &params);
    free_resources(threads, forks, philos);
    return (0);
}
