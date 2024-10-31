#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>   // For usleep
#include <sys/time.h> // For gettimeofday


typedef struct s_params
{
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    struct timeval start_time;
    int stop;
    pthread_mutex_t stop_mutex;
} t_params;

typedef struct s_philosopher
{
    int id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_params *params;
    struct timeval last_meal_time;
} t_philosopher;


long get_timestamp(struct timeval start_time)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long seconds = current_time.tv_sec - start_time.tv_sec;
    long microseconds = current_time.tv_usec - start_time.tv_usec;
    return (seconds * 1000) + (microseconds / 1000);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    // Determine the order of forks to pick up to avoid deadlock
    if (philo->id % 2 == 0)
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
    else
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }

    // Initialize last meal time
    gettimeofday(&philo->last_meal_time, NULL);

    while (1)
    {
        // Check if simulation has been stopped
        pthread_mutex_lock(&philo->params->stop_mutex);
        if (philo->params->stop)
        {
            pthread_mutex_unlock(&philo->params->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->params->stop_mutex);

        // Thinking
        printf("%ld %d is thinking\n", get_timestamp(philo->params->start_time), philo->id);

        // Picking up forks
        pthread_mutex_lock(first_fork);
        printf("%ld %d has taken a fork\n", get_timestamp(philo->params->start_time), philo->id);

        pthread_mutex_lock(second_fork);
        printf("%ld %d has taken a fork\n", get_timestamp(philo->params->start_time), philo->id);

                // Check if philosopher has died
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        long time_since_last_meal = (current_time.tv_sec - philo->last_meal_time.tv_sec) * 1000 +
                                    (current_time.tv_usec - philo->last_meal_time.tv_usec) / 1000;
        if (time_since_last_meal >= philo->params->time_to_die)
        {
            printf("%ld %d died\n", get_timestamp(philo->params->start_time), philo->id);
            // Set the stop flag
            pthread_mutex_lock(&philo->params->stop_mutex);
            philo->params->stop = 1;
            pthread_mutex_unlock(&philo->params->stop_mutex);
            // Terminate the simulation gracefully
            break;
        }

        // Eating
        gettimeofday(&philo->last_meal_time, NULL);
        printf("%ld %d is eating\n", get_timestamp(philo->params->start_time), philo->id);
        usleep(philo->params->time_to_eat * 1000);

        // Putting down forks
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);

        // Sleeping
        printf("%ld %d is sleeping\n", get_timestamp(philo->params->start_time), philo->id);
        usleep(philo->params->time_to_sleep * 1000);
    }
    return NULL;
}


int main()
{
    pthread_t philo_thread1;
    pthread_t philo_thread2;
    pthread_mutex_t fork1;
    pthread_mutex_t fork2;
    t_philosopher philo1;
    t_philosopher philo2;
    t_params params;

    // Initialize forks (mutexes)
    pthread_mutex_init(&fork1, NULL);
    pthread_mutex_init(&fork2, NULL);

    // Initialize simulation parameters
    params.time_to_die = 2000;   // 2000 milliseconds
    params.time_to_eat = 500;    // 500 milliseconds
    params.time_to_sleep = 500;  // 500 milliseconds
    gettimeofday(&params.start_time, NULL);
    params.stop = 0;
    pthread_mutex_init(&params.stop_mutex, NULL);

    // Initialize philosopher 1
    philo1.id = 1;
    philo1.left_fork = &fork1;
    philo1.right_fork = &fork2;
    philo1.params = &params;

    // Initialize philosopher 2
    philo2.id = 2;
    philo2.left_fork = &fork2;
    philo2.right_fork = &fork1;
    philo2.params = &params;

    // Create threads for each philosopher
    pthread_create(&philo_thread1, NULL, philosopher_routine, &philo1);
    pthread_create(&philo_thread2, NULL, philosopher_routine, &philo2);

    // Wait for threads to finish
    pthread_join(philo_thread1, NULL);
    pthread_join(philo_thread2, NULL);

    // Destroy mutexes
    pthread_mutex_destroy(&fork1);
    pthread_mutex_destroy(&fork2);
    pthread_mutex_destroy(&params.stop_mutex);

    return 0;
}