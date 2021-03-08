NAME=cub3D
SRCS = cube3d.c GNL/get_next_line.c GNL/get_next_line_utils.c dda.c inits.c moves.c draw_sprites.c error_handler.c \
		floor_ceiling.c ft_substr.c parser.c simplest_drawing.c
OBJS = $(SRCS:.c=.o)
LIBS = libft.a libmlx.dylib
INCLUDES = cub3D.h
all: $(SRCS) $(NAME)
$(NAME): $(OBJS)
	make -C libft/ bonus
	cp libft/libft.a .
	make -C minilibx/
	cp minilibx/libmlx.dylib .
	gcc -framework OpenGL -framework AppKit -o $(NAME) $(OBJS) $(LIBS)
.c.o:
	gcc -Iminilibx -Ilibft -c $< -o $@
clean:
	make -C libft/ fclean
	make -C minilibx/ clean
	rm -f $(OBJS) $(LIBS)
fclean: clean
	rm -f $(NAME)
re: fclean all
debug:
	gcc -g -framework OpenGL -framework AppKit -Ilibft -Iminilibx $(SRCS) $(LIBS) -o $(NAME)
.PHONY: clean fclean re debug run all
