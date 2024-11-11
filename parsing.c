#include "philo.h"

// Prints an error message in red color and returns 1
int	ft_error(char *message)
{
	int	i;

	i = 0;
	while (message[i])
		i++;
	// RED and RESET are macros defined for color formatting
	printf("%s%s%s\n", RED, message, RESET);
	return (1);
}

// Checks if the given string contains only numeric characters
static int	is_numeric_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ('0' <= str[i] && str[i] <= '9')
			i++;
		else
			return (0); // Non-numeric character found
	}
	return (1); // String is purely numeric
}

// Validates the command-line arguments for correctness
int	validate_arguments(int argc, char **argv)
{
	int	i;

	// Check if the number of arguments is either 5 or 6
	if (argc < 5 || argc > 6)
		return (ft_error("Invalid number of arguments.") - 1);
	i = 1;
	// Loop through each argument to verify it's numeric
	while (i < argc)
	{
		if (!is_numeric_string(argv[i]))
			return (ft_error("Some arguments are not numbers.") - 1);
		i++;
	}
	return (1); // All arguments are valid
}
