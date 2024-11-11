########################################
########## VARIABLES

NAME		= philo
CC			= cc
CFLAGS		= -Wall -Werror -Wextra
RM			= rm -f

########################################
########## COLORS
DEF_COLOR = \033[0;39m
GRAY = \033[1;90m
RED = \033[1;91m
GREEN = \033[1;92m
YELLOW = \033[1;93m
BLUE = \033[1;94m
MAGENTA = \033[1;95m
CYAN = \033[1;96m
WHITE = \033[1;97m

########################################
########## SOURCES
SRCS	=	main.c utils.c parsing.c philo_life.c

OBJS	=	$(SRCS:.c=.o)

.c.o:
		@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
		@echo "$(MAGENTA)Compilation of : $<..$(DEF_COLOR)"

########################################
########## RULES #DEBUG : -fsanitize=thread -g

all:		$(NAME)

$(NAME):	$(OBJS)
				@$(CC) $(OBJS) -o $(NAME)
				@echo "$(GREEN)Success compilation of $(NAME) !$(DEF_COLOR)"

clean:
				@$(RM) $(OBJS)
				@echo "$(YELLOW)rm of $(OBJS).$(DEF_COLOR)"

fclean: 	clean
				@$(RM) $(NAME)
				@echo "$(RED)rm of $(NAME).$(DEF_COLOR)"



re:			fclean all

norm:
			norminette $(SRCS) | grep -v Norme -B1 || true

.PHONY: all clean fclean re norm