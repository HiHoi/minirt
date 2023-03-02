# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hoslim <hoslim@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/22 20:01:46 by hoslim            #+#    #+#              #
#    Updated: 2023/03/02 21:20:12 by hoslim           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test
SRCS = \
		./main.c\
		./ray.c\
		./scene.c\
		./utils.c\
		./create_obj.c\
		./hit.c

HEADER = includes
OBJS = $(SRCS:.c=.o)
CC = gcc
FLAGS = -Wall -Werror -Wextra
MLX_NAME = mlx
MLX_DIR = ./mlx
MLX = $(addprefix $(MLX_DIR)/, libmlx.a)
CLIB = -Lmlx -lmlx -framework OpenGL -framework Appkit -Imlx

all : $(NAME)

%.o : %.c
	$(CC) $(FLAGS) -I $(HEADER) -I$(MLX_DIR) -c $< -o $@

$(NAME) : $(OBJS)
	$(MAKE) -C $(MLX_DIR) all
	$(CC) $(FLAGS) $(CLIB) -o $(NAME) $(OBJS) -L$(MLX_DIR) -l$(MLX_NAME)
	install_name_tool -change libmlx.dylib mlx/libmlx.dylib $(NAME)

clean :
	rm -rf *.o

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all re clean fclean
