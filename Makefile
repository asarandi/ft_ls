# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/26 20:09:21 by asarandi          #+#    #+#              #
#    Updated: 2017/12/03 20:29:06 by asarandi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ft_ls
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -O3
SRC		= $(wildcard *.c)
OBJ		= $(SRC:%.c=%.o)
LIB		= -Llibft -lft -Lft_printf -lftprintf

all: $(NAME)

$(NAME):
	@make -C libft
	@make -C ft_printf
	$(info $(NAME):		compiling .o files)
	@$(CC) $(CFLAGS) -c $(SRC)
	$(info $(NAME):		building executable)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

debug: fclean
	@make debug -C libft
	@make debug -C ft_printf
	$(info $(NAME):		compiling debug .o files)
	@$(CC) -g -c $(SRC)
	$(info $(NAME):		building debug executable)
	@$(CC) -g $(OBJ) $(LIB) -o debug

build:
	$(info $(NAME):	building executable)
	@rm -rf $(OBJ) $(NAME)
	@$(CC) $(CFLAGS) -c $(SRC)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

clean:
	@make clean -C libft
	@make clean -C ft_printf
	$(info $(NAME):		removing .o files)
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C libft
	@make fclean -C ft_printf
	$(info $(NAME):		removing $(NAME))
	@rm -rf $(NAME) debug

re: fclean all
