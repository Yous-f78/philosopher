#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>   // For usleep
# include <sys/time.h> // For gettimeofday

typedef struct s_params
{
    int             num_philosophers;
    int             times_must_eat;
    int             finished_philosophers;
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
    int             meals_eaten;
} t_philosopher;

// Function prototypes from ft_atol.c
long    ft_atol(const char *str, int *error);
int     skip_whitespace(const char *str, int *i);
int     parse_sign(const char *str, int *i);
int     is_numeric_string(const char *str, int i);

// Function prototypes from args.c
int     parse_arguments(int argc, char **argv, t_params *params);
long    parse_positive_long(const char *str, int *error_flag);
int     print_error(void);
int     print_usage(void);

// Function prototypes from main.c
void    init_params(t_params *params);

// Function prototypes from setup.c
int     allocate_resources(pthread_t **threads, pthread_mutex_t **forks,
                           t_philosopher **philos, t_params *params);
void    init_mutexes(pthread_mutex_t *forks, t_params *params);
void    init_philosophers(t_philosopher *philos, pthread_mutex_t *forks, t_params *params);
void    create_philosopher_threads(pthread_t *threads, t_philosopher *philos, int num_philosophers);

// Function prototypes from cleanup.c
void    join_threads(pthread_t *threads, int num_philosophers);
void    destroy_mutexes(pthread_mutex_t *forks, t_params *params);
void    free_resources(pthread_t *threads, pthread_mutex_t *forks, t_philosopher *philos);

// Function prototypes from philosopher.c
void    *philosopher_routine(void *arg);
int     check_death(t_philosopher *philo);
int     check_meals(t_philosopher *philo);
void    handle_single_philosopher(t_philosopher *philo, pthread_mutex_t *fork);

// Function prototypes from actions.c
void    take_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void    eat(t_philosopher *philo);
void    put_down_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);

// Function prototypes from utils.c
void    set_fork_order(t_philosopher *philo, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork);
void    print_status(t_philosopher *philo, char *message);
long    get_timestamp(struct timeval start_time);
void    precise_sleep(t_philosopher *philo, long sleep_time_ms);
int     check_stop(t_philosopher *philo); // ATTENTION 6 FONCTIONS

#endif
