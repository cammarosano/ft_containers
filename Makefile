SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
HEADER = ft_vector.hpp
CC = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = a.out

all:	$(NAME)

test:	all
		./$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $^ -o $@

%.o:		%.cpp $(HEADER)
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
