NAME	=	ft_irc
O		=	obj
I		=	inc
S		=	src
SRC		=	$S/Server.cpp \
			$S/Channel.cpp \
			$S/Client.cpp \
			$S/utils.cpp \
			$S/main.cpp
INCS	=	$I/Server.hpp \
			$I/Channel.hpp \
			$I/Client.hpp \
			$I/rpl.hpp
OBJ		=	$(SRC:$S/%.cpp=$O/%.o)
CC		=	clang++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -I$I -g
LDFLAGS	=

ERASE		=	\033[2K\r
BLUE		=	\033[34m
YELLOW		=	\033[33m
GREEN		=	\033[32m
END			=	\033[0m

all: $(NAME)

$O:
	@mkdir -p $O

$O/%.o: $S/%.cpp Makefile $(INCS) | $O
	$(CC) $(CFLAGS) -c $< -o $@
	printf "$(BLUE)-> $<$(END)\n"

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)
	printf "$(GREEN)$(NAME) made$(END)\n"

clean:
	rm -rdf $O
	printf "$(YELLOW)$O removed$(END)\n"

fclean:	clean
	rm -f $(NAME)
	printf "$(YELLOW)$(NAME) removed$(END)\n"

re: fclean all

.PHONY: all clean fclean re
.SILENT:
