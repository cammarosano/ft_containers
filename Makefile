SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
HEADER = ft_vector.hpp vector_iterator.hpp reverse_iterator.hpp ft_map.hpp 
CC = clang++
CFLAGS = -Wall -Wextra -Werror -g  -fsanitize=address
NAME = a.out

all:	$(NAME)

unit_test:	$(HEADER)
			@	# @ $(CC) $(CFLAGS) unit_test.cpp -lcriterion -o $@ # linux	
			@ $(CC) $(CFLAGS) unit_test.cpp -lcriterion -o $@ -I ~/.brew/include -L ~/.brew/lib  -std=c++11
			@ ./$@

map_test:	ft_map.hpp map_iterator.hpp utils.hpp
			@ $(CC) $(CFLAGS) unit_test_map.cpp -lcriterion -o $@ -I ~/.brew/include -L ~/.brew/lib  -std=c++11
			@ ./$@

my_map_test:	my_map_test.o
			$(CC) $(CFLAGS) $^ -o $@

rb_tree_test:	Rbtree.hpp
			@  $(CC) $(CFLAGS) unit_test_Rbtree.cpp -lcriterion -o $@
			@ ./$@

new_map_test:	
			@  $(CC) $(CFLAGS) unit_test_new_map.cpp -lcriterion -o $@ -I ~/.brew/include -L ~/.brew/lib  -std=c++11
			@ ./$@


$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $^ -o $@

%.o:		%.cpp $(HEADER)
			$(CC) $(CFLAGS) -std=c++98 -c $< -o $@

clean:
			rm -f $(OBJ) my_map_test.o

fclean:		clean
			rm -f $(NAME) unit_test map_test my_map_test rb_tree_test new_map_test

re:			fclean all

.PHONY:		all clean fclean re unit_test map_test rb_tree_test new_map_test
