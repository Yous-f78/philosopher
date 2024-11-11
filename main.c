#include "philo.h"

// Initialize individual philosopher data
static void initialize_philosopher(t_philosopher *philosopher, t_simulation *simulation, int index)
{
    // Link the philosopher to the shared simulation data
    philosopher->simulation = simulation;
    philosopher->meals_eaten = 0;
    philosopher->right_fork = NULL;
    philosopher->id = index + 1;
    philosopher->last_meal_time = timestamp();

    // Assign the right fork (the left fork of the next philosopher)
    if (index == simulation->philosopher_count - 1)
        // If last philosopher, the right fork is the first philosopher's left fork (circular table)
        philosopher->right_fork = &simulation->philosophers[0].left_fork;
    else
        // Otherwise, the right fork is the next philosopher's left fork
        philosopher->right_fork = &simulation->philosophers[index + 1].left_fork;
}

// Initialize all philosophers and start their threads
static void initialize_philosophers(t_simulation *simulation)
{
    int index = 0;

    // Record the simulation start time
    simulation->start_time = timestamp();

    // Initialize mutexes for each philosopher's left fork
    while (index < simulation->philosopher_count)
        pthread_mutex_init(&simulation->philosophers[index++].left_fork, NULL);

    index = 0;
    while (index < simulation->philosopher_count)
    {
        // Initialize individual philosopher data
        initialize_philosopher(&simulation->philosophers[index], simulation, index);

        // Create a thread for each philosopher
        if (pthread_create(&simulation->philosophers[index].thread, NULL,
                           &philosopher_routine, &simulation->philosophers[index]) != 0)
            break;

        // Slight delay to stagger the start of each thread
        usleep(200);
        index++;
    }

    // Wait for all philosopher threads to finish
    index = 0;
    while (index < simulation->philosopher_count)
        pthread_join(simulation->philosophers[index++].thread, NULL);
}

// Initialize simulation data from command-line arguments
static void initialize_simulation(t_simulation *simulation, char **args, int argc)
{
    // Parse command-line arguments to set simulation parameters
    simulation->philosopher_count = ft_atoi(args[0]);
    simulation->time_to_die = ft_atoi(args[1]);
    simulation->time_to_eat = ft_atoi(args[2]);
    simulation->time_to_sleep = ft_atoi(args[3]);
    simulation->someone_died = 0;

    // If the number of meals is specified, set it; otherwise, use -1 (infinite)
    if (argc == 6)
        simulation->meals_required = ft_atoi(args[4]);
    else
        simulation->meals_required = -1;

    // Initialize mutexes for printing and death checking
    pthread_mutex_init(&simulation->print_mutex, NULL);
    pthread_mutex_init(&simulation->death_mutex, NULL);

    // Allocate memory for all philosophers
    simulation->philosophers = malloc(sizeof(t_philosopher) * simulation->philosopher_count);
    if (!simulation->philosophers)
        return;

    // Initialize philosophers and start simulation
    initialize_philosophers(simulation);
}

// Clean up resources after the simulation ends
static void cleanup_simulation(t_simulation *simulation)
{
    int index = 0;

    // Destroy mutexes for each philosopher's left fork
    while (index < simulation->philosopher_count)
        pthread_mutex_destroy(&simulation->philosophers[index++].left_fork);

    // Destroy shared mutexes
    pthread_mutex_destroy(&simulation->print_mutex);
    pthread_mutex_destroy(&simulation->death_mutex);

    // Free allocated memory for philosophers
    free(simulation->philosophers);
}

int main(int argc, char **argv)
{
    t_simulation simulation;

    // Validate command-line arguments
    if (!validate_arguments(argc, argv))
        return (1);

    // Initialize simulation data and start the simulation
    initialize_simulation(&simulation, &argv[1], argc);

    // Clean up resources after simulation ends
    cleanup_simulation(&simulation);

    return (0);
}
