#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// Language setting: Set to 1 for French output, 0 for English
#define FRENCH 0

// Color settings: Set to 1 to enable colored output, 0 to disable
#define COLORS 1

#if COLORS == 1
    #define RED     "\033[1;31m"
    #define BLUE    "\033[1;34m"
    #define GREEN   "\033[1;32m"
    #define YELLOW  "\033[1;33m"
    #define CYAN    "\033[1;36m"
    #define WHITE   "\033[1;37m"
    #define ORANGE  "\033[1;38;5;208m"
    #define PURPLE  "\033[38;5;129m"
    #define RESET   "\033[0m"
#else
    #define RED     "\033[0m"
    #define BLUE    "\033[0m"
    #define GREEN   "\033[0m"
    #define YELLOW  "\033[0m"
    #define CYAN    "\033[0m"
    #define WHITE   "\033[0m"
    #define ORANGE  "\033[0m"
    #define PURPLE  "\033[0m"
    #define RESET   "\033[0m"
#endif

// Structure representing a philosopher
typedef struct s_philosopher
{
    int                 id;             // Philosopher's unique identifier
    int                 meals_eaten;    // Number of meals eaten
    long int            last_meal_time; // Timestamp of the last meal
    pthread_t           thread;         // Thread representing the philosopher
    pthread_mutex_t     left_fork;      // Mutex for the left fork
    pthread_mutex_t     *right_fork;    // Pointer to the right fork mutex
    struct s_simulation *simulation;    // Pointer to the shared simulation data
}                       t_philosopher;

// Structure containing simulation data
typedef struct s_simulation
{
    int                 philosopher_count;  // Total number of philosophers
    int                 time_to_die;        // Time (in ms) a philosopher can live without eating
    int                 time_to_eat;        // Time (in ms) a philosopher takes to eat
    int                 time_to_sleep;      // Time (in ms) a philosopher sleeps
    int                 meals_required;     // Number of meals each philosopher must eat (-1 if infinite)
    int                 someone_died;       // Flag indicating if a philosopher has died
    long int            start_time;         // Timestamp when the simulation started
    pthread_mutex_t     print_mutex;        // Mutex to protect printing to the console
    pthread_mutex_t     death_mutex;        // Mutex to protect death flag
    t_philosopher       *philosophers;      // Array of philosopher structures
}                       t_simulation;

// Function prototypes

// Validates the command-line arguments
int         validate_arguments(int argc, char **argv);

// Prints an error message and returns 1
int         ft_error(char *message);

// Converts a string to an integer
int         ft_atoi(const char *str);

// Returns the current timestamp in milliseconds
long int    timestamp(void);

// The routine executed by each philosopher thread
void        *philosopher_routine(void *philosopher_param);

// Sleeps for a specified number of milliseconds
int         sleep_ms(int ms);

// Prints the philosopher's action
void        print_action(t_philosopher *philosopher, char action);

// Checks if the philosopher should die based on the time since last meal
void        check_death(t_philosopher *philosopher);

#endif
