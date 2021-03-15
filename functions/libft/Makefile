# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/01 18:41:05 by plurlene          #+#    #+#              #
#    Updated: 2020/11/05 14:22:44 by plurlene         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= ft_putendl_fd.c ft_isdigit.c ft_memmove.c ft_strdup.c ft_strnstr.c ft_atoi.c ft_isprint.c ft_memset.c ft_strjoin.c ft_strrchr.c ft_bzero.c ft_itoa.c ft_putchar_fd.c ft_strlcat.c ft_strtrim.c ft_calloc.c ft_memccpy.c ft_putnbr_fd.c ft_strlcpy.c ft_substr.c ft_isalnum.c ft_memchr.c ft_putstr_fd.c ft_strlen.c ft_tolower.c ft_isalpha.c ft_memcmp.c ft_split.c ft_strmapi.c ft_toupper.c ft_isascii.c ft_memcpy.c ft_strchr.c ft_strncmp.c
OBJ		= $(SRCS:.c=.o)
BNS		= ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c
BNSOBJ	= $(BNS:.c=.o)
NAME	= libft.a
HEADER	= libft.h
CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

.c.o :
			$(CC) $(FLAGS) -c $< -o $(<:.c=.o) -I$(HEADER)

all: $(NAME)

$(NAME):	$(OBJ)
			ar rc $(NAME) $(OBJ)

bonus:		$(BNSOBJ) $(OBJ)
			ar rc $(NAME) $(OBJ) $(BNSOBJ)
clean:
			$(RM) $(OBJ) $(BNSOBJ)

fclean:		clean
			$(RM) $(NAME)

re:		fclean all
