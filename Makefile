NAME=cub3D
SRCS = ./functions/cube3d.c ./functions/GNL/get_next_line.c ./functions/GNL/get_next_line_utils.c ./functions/dda.c ./functions/inits.c ./functions/moves.c ./functions/draw_sprites.c ./functions/error_handler.c \
		./functions/floor_ceiling.c ./functions/ft_substr.c ./functions/parser.c ./functions/simplest_drawing.c ./functions/screenshot.c ./functions/other.c ./functions/color_texture_resolution_parser.c\
		./functions/file_and_map_parser.c ./functions/player_and_sprite_parser.c
OBJS = $(SRCS:.c=.o)
LIBS = libft.a libmlx.dylib
INCLUDES = ./functions/cube3d.h
all: $(SRCS) $(NAME)
$(NAME): $(OBJS)
	make -C ./functions/libft/ bonus
	cp ./functions/libft/libft.a .
	make -C minilibx/
	cp minilibx/libmlx.dylib .
	gcc -framework OpenGL -framework AppKit -o $(NAME) $(OBJS) $(LIBS)
.c.o:
	gcc -Iminilibx -I./functions/libft -I$(INCLUDES) -c $< -o $@
clean:
	make -C ./functions/libft/ fclean
	make -C minilibx/ clean
	rm -f $(OBJS) $(LIBS)
fclean: clean
	rm -f $(NAME)
re: fclean all
debug:
	gcc -g -framework OpenGL -framework AppKit -Ilibft -Iminilibx $(SRCS) $(LIBS) -o $(NAME)
.PHONY: clean fclean re debug run all
