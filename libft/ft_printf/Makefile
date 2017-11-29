# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/30 09:00:12 by asarandi          #+#    #+#              #
#    Updated: 2017/11/26 22:45:40 by asarandi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libftprintf.a
CFLAGS	= -Wall -Werror -Wextra -I. -c
FILES	=	add_precision.c \
			add_width.c \
			format_ioux.c \
			format_csws.c \
			ft_printf.c \
			get_char.c \
			get_double.c \
			non_numeric.c \
			get_specs.c \
			init_lib.c \
			is_format.c \
			libft_functions.c \
			print_number.c \
			string_op.c \
			wchar_to_utf8.c \
			iso8601.c \
			colors.c \
			float_cosmetics.c

OBJ		= $(FILES:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(info $(NAME):	building library)
	@ar rcs $(NAME) $(OBJ)

norm:
	@norminette $(FILES) libftprintf.h

$(OBJ): $(FILES)
	$(info $(NAME):	compiling .o files)
	@gcc $(CFLAGS) $(FILES)

debug: fclean
	$(info $(NAME):	compiling debug .o files)
	@gcc -g $(CFLAGS) $(FILES)
	$(info $(NAME):	building debug library)
	@ar rcs $(NAME) $(OBJ)

clean:
	$(info $(NAME):	removing .o files)
	@rm -f $(OBJ)

fclean: clean
	$(info $(NAME):	removing $(NAME))
	@rm -f $(NAME)

re: fclean all
