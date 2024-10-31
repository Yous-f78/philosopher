#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>   // For usleep
# include <sys/time.h> // For gettimeofday

typedef struct s_params
{
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    struct timeval  start_time;
    int             stop;
    pthread_mutex_t stop_mutex;
} t_params;

typedef struct s_philosopher
{
    int             id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_params        *params;
    struct timeval  last_meal_time;
} t_philosopher;

// Function prototypes from philosopher.c
void    *philosopher_routine(void *arg);
int     check_death(t_philosopher *philo);
void    take_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void    eat(t_philosopher *philo);
void    put_down_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);

// Function prototypes from utils.c
void    set_fork_order(t_philosopher *philo, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork);
void    print_status(t_philosopher *philo, char *message);
long    get_timestamp(struct timeval start_time);
void    precise_sleep(long sleep_time_ms);

// Function prototypes from main.c
void    init_mutexes(pthread_mutex_t *fork1, pthread_mutex_t *fork2, t_params *params);
void    init_params(t_params *params);
void    init_philosophers(t_philosopher *philo1, t_philosopher *philo2,
                          pthread_mutex_t *fork1, pthread_mutex_t *fork2, t_params *params);
void    create_philosopher_threads(pthread_t *philo_thread1, pthread_t *philo_thread2,
                                   t_philosopher *philo1, t_philosopher *philo2);

#endif
