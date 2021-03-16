NAME=cub3D
NAMEBNS=cub3DBonus
SRCS = ./functions/cube3d.c ./functions/GNL/get_next_line.c ./functions/GNL/get_next_line_utils.c ./functions/dda.c ./functions/inits.c ./functions/moves.c ./functions/draw_sprites.c ./functions/error_handler.c \
		./functions/floor_ceiling.c ./functions/ft_substr.c ./functions/parser.c ./functions/simplest_drawing.c ./functions/screenshot.c ./functions/other.c ./functions/color_texture_resolution_parser.c\
		./functions/file_and_map_parser.c ./functions/player_and_sprite_parser.c
BNS = ./functions/cube3d.c ./functions/GNL/get_next_line.c ./functions/GNL/get_next_line_utils.c ./functions/dda_bonus.c ./functions/inits.c ./functions/moves.c ./functions/draw_sprites_bonus.c ./functions/error_handler.c \
		./functions/floor_ceiling.c ./functions/ft_substr.c ./functions/parser.c ./functions/simplest_drawing.c ./functions/screenshot.c ./functions/other.c ./functions/color_texture_resolution_parser.c\
		./functions/file_and_map_parser.c ./functions/player_and_sprite_parser.c
BNSOBJ = $(BNS:.c=.o)
OBJS = $(SRCS:.c=.o)
INCLUDES = ./functions/cube3d.h
all: $(SRCS) $(NAME)
$(NAME): $(INCLUDES) $(OBJS)
	make -C ./functions/libft/
	cp ./functions/libft/libft.a .
	make -C minilibx/
	cp minilibx/libmlx.dylib .
	gcc -framework OpenGL -framework AppKit -o $(NAME) $(OBJS) libft.a libmlx.dylib
.c.o:
	gcc -Iminilibx -I./functions/libft -I$(INCLUDES) -c $< -o $@
clean:
	make -C ./functions/libft/ fclean
	make -C minilibx/ clean
	rm -f $(OBJS) $(BNSOBJ) libft.a
fclean: clean
	rm -f $(NAME) $(NAMEBNS) libmlx.dylib
re: fclean all
bonus: $(BNS) $(NAMEBNS)
$(NAMEBNS): $(INCLUDES) $(BNSOBJ)
	make -C ./functions/libft/
	cp ./functions/libft/libft.a .
	make -C minilibx/
	cp minilibx/libmlx.dylib .
	gcc -framework OpenGL -framework AppKit -o $(NAMEBNS) $(BNSOBJ) libft.a libmlx.dylib
debug:
	gcc -g -framework OpenGL -framework AppKit -Ilibft -Iminilibx $(SRCS) libft.a libmlx.dylib -o $(NAME)
.PHONY: clean fclean re debug run all
