#include "philo.h"

// Function for the philosopher to grab forks
static int grab_forks(t_philosopher *philosopher, t_simulation *simulation)
{
    if (philosopher->id % 2 != 0)
    {
        // Odd-numbered philosophers pick up the right fork first
        pthread_mutex_lock(philosopher->right_fork);
        check_death(philosopher);
        if (simulation->someone_died >= 1)
        {
            pthread_mutex_unlock(philosopher->right_fork);
            return (0);
        }
        print_action(philosopher, 'f'); // 'f' for fork
        pthread_mutex_lock(&philosopher->left_fork);
        print_action(philosopher, 'f');
        return (1);
    }
    // Even-numbered philosophers pick up the left fork first
    pthread_mutex_lock(&philosopher->left_fork);
    check_death(philosopher);
    if (simulation->someone_died >= 1)
    {
        pthread_mutex_unlock(&philosopher->left_fork);
        return (0);
    }
    print_action(philosopher, 'f');
    pthread_mutex_lock(philosopher->right_fork);
    print_action(philosopher, 'f');
    return (1);
}

// Function for the philosopher to eat
static void philosopher_eat(t_philosopher *philosopher)
{
    t_simulation *simulation = philosopher->simulation;

    if (grab_forks(philosopher, simulation) == 0)
        return;

    print_action(philosopher, 'e'); // 'e' for eating
    philosopher->meals_eaten++;
    sleep_ms(simulation->time_to_eat);
    philosopher->last_meal_time = timestamp();

    // Release the forks after eating
    if (philosopher->id % 2 != 0)
    {
        pthread_mutex_unlock(&philosopher->left_fork);
        pthread_mutex_unlock(philosopher->right_fork);
    }
    else
    {
        pthread_mutex_unlock(philosopher->right_fork);
        pthread_mutex_unlock(&philosopher->left_fork);
    }
}

// Function to perform an action based on the specified character
static void philosopher_action(t_philosopher *philosopher, char action)
{
    if (action == 's') // 's' for sleeping
    {
        if (philosopher->simulation->someone_died >= 1)
            return;
        print_action(philosopher, 's');
        sleep_ms(philosopher->simulation->time_to_sleep);
    }
    else if (action == 't') // 't' for thinking
    {
        if (philosopher->simulation->someone_died >= 1)
            return;
        print_action(philosopher, 't');
    }
    else if (action == 'e') // 'e' for eating
    {
        philosopher_eat(philosopher);
    }
}

// Checks if the philosopher should die based on the time since last meal
void check_death(t_philosopher *philosopher)
{
    t_simulation *simulation = philosopher->simulation;
    long int current_time;

    pthread_mutex_lock(&simulation->death_mutex);
    current_time = timestamp();

    // Check if the philosopher has exceeded the time to die
    if (philosopher->last_meal_time + simulation->time_to_die < current_time + simulation->time_to_eat)
    {
        if (simulation->someone_died == 0)
            print_action(philosopher, 'd'); // 'd' for died
        simulation->someone_died++;
        pthread_mutex_unlock(&simulation->death_mutex);
        return;
    }
    pthread_mutex_unlock(&simulation->death_mutex);
}

// Main routine for each philosopher thread
void *philosopher_routine(void *philosopher_param)
{
    t_philosopher *philosopher = (t_philosopher *)philosopher_param;
    int meal_count = 0;

    while (1)
    {
        // Check if any philosopher has died
        if (philosopher->simulation->someone_died >= 1)
            break;

        // Special case for a single philosopher
        if (philosopher->simulation->philosopher_count == 1)
        {
            sleep_ms(philosopher->simulation->time_to_die);
            print_action(philosopher, 'd');
            break;
        }

        // Check if the philosopher has eaten the required number of meals
        if (philosopher->simulation->meals_required != -1 && meal_count == philosopher->simulation->meals_required)
            break;

        // Philosophers think before eating (after the first iteration)
        if (meal_count != 0)
            philosopher_action(philosopher, 't');

        // Philosopher attempts to eat
        philosopher_action(philosopher, 'e');

        // Philosopher sleeps after eating
        philosopher_action(philosopher, 's');

        meal_count++;
    }
    return NULL;
}
