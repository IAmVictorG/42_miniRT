NAME = miniRT

INCLUDES = includes

LINUX_FLAGS = -lm -lX11 -lXext -lpthread

SRCS =  $(addprefix srcs/, main.c utils_init.c events.c render.c camera.c) \
		$(addprefix srcs/parsing/, parse_utils.c parse.c) \
		$(addprefix srcs/utils/, utils.c utils_vec3.c)

LIBS = $(addprefix includes/, libft.a libftprintf.a)
OBJS	= ${SRCS:.c=.o}
CFLAGS	=  -Wall -Wextra -Werror
CC		= gcc
RM		= rm -f

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	make -C includes/minilibx-linux
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) includes/minilibx-linux/libmlx.a -fsanitize=address $(LINUX_FLAGS) -o $(NAME)


all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re