#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>   // For usleep
#include <sys/time.h> // For gettimeofday

typedef struct s_philosopher
{
    int id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
} t_philosopher;

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    while (1)
    {
        // Thinking
        printf("Philosopher %d is thinking.\n", philo->id);

        // Trying to pick up left fork
        pthread_mutex_lock(philo->left_fork);
        printf("Philosopher %d picked up left fork.\n", philo->id);

        // Trying to pick up right fork
        pthread_mutex_lock(philo->right_fork);
        printf("Philosopher %d picked up right fork.\n", philo->id);

        // Eating
        printf("Philosopher %d is eating.\n", philo->id);
        usleep(1000 * 500); // Simulate eating for 500ms

        // Putting down right fork
        pthread_mutex_unlock(philo->right_fork);
        printf("Philosopher %d put down right fork.\n", philo->id);

        // Putting down left fork
        pthread_mutex_unlock(philo->left_fork);
        printf("Philosopher %d put down left fork.\n", philo->id);

        // Sleeping
        printf("Philosopher %d is sleeping.\n", philo->id);
        usleep(1000 * 500); // Simulate sleeping for 500ms
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

    // Initialize forks (mutexes)
    pthread_mutex_init(&fork1, NULL);
    pthread_mutex_init(&fork2, NULL);

    // Initialize philosopher 1
    philo1.id = 1;
    philo1.left_fork = &fork1;
    philo1.right_fork = &fork2;

    // Initialize philosopher 2
    philo2.id = 2;
    philo2.left_fork = &fork2;
    philo2.right_fork = &fork1;

    // Create threads for each philosopher
    pthread_create(&philo_thread1, NULL, philosopher_routine, &philo1);
    pthread_create(&philo_thread2, NULL, philosopher_routine, &philo2);

    // Wait for threads to finish (they won't in this infinite loop, so we can join or detach)
    pthread_join(philo_thread1, NULL);
    pthread_join(philo_thread2, NULL);

    // Destroy mutexes (unreachable in this code, but good practice)
    pthread_mutex_destroy(&fork1);
    pthread_mutex_destroy(&fork2);

    return 0;
}