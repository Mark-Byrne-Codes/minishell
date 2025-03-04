# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 14:57:50 by mbyrne            #+#    #+#              #
#    Updated: 2024/12/27 08:44:14 by mbyrne           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                Makefile                                      #
# **************************************************************************** #

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC

# Source files for libft
LIBFT_SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
            ft_toupper.c ft_tolower.c ft_strlen.c ft_strdup.c ft_strlcpy.c \
            ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c \
            ft_atoi.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
            ft_memchr.c ft_memcmp.c ft_calloc.c ft_substr.c \
            ft_strjoin.c ft_strtrim.c ft_putnbr_fd.c ft_putchar_fd.c \
            ft_putstr_fd.c ft_putendl_fd.c ft_split.c ft_itoa.c \
            ft_strmapi.c ft_striteri.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
            ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
            ft_lstclear.c ft_lstiter.c ft_issign.c ft_isspace.c \
            get_next_line.c ft_printf.c ft_printf_digits.c ft_printf_char.c



# Combined source files
SRC = $(LIBFT_SRC)

# Object files
OBJ = $(SRC:.c=.o)

all: $(NAME)

.SILENT:

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

