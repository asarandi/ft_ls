# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/26 20:09:21 by asarandi          #+#    #+#              #
#    Updated: 2017/11/27 01:55:34 by asarandi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ft_ls
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra
SRC		= $(wildcard *.c)
OBJ		= $(SRC:%.c=%.o)
LIB		= -Llibft -lft -Llibft/ft_printf -lftprintf

all: $(NAME)

$(NAME):
	@make -C libft
	@make -C libft/ft_printf
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

debug: fclean
	@make debug -C libft
	@make debug -C libft/ft_printf
	$(CC) -g -c $(SRC)
	$(CC) -g $(OBJ) $(LIB) -o debug

build:
	rm -rf $(OBJ) $(NAME)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

clean:
	@make clean -C libft
	@make clean -C libft/ft_printf
	rm -rf $(OBJ)

fclean: clean
	@make fclean -C libft
	@make fclean -C libft/ft_printf
	rm -rf $(NAME) debug

re: fclean all
