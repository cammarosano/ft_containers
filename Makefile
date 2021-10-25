SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
HEADER = ft_vector.hpp vector_iterator.hpp reverse_iterator.hpp ft_map.hpp # not updated
INCLUDE = -I includes -I containers
CRITERION = -l criterion -I ~/.brew/include -L ~/.brew/lib -std=c++11
CC = clang++
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address $(INCLUDE)
NAME = test
UNIT_TESTS = vector_test map_test_0 map_test_1 rb_tree_test set_test stack_test

all:	$(NAME)

$(NAME):
			@ echo "====== Testing FT containers ======="
			@ $(CC) $(CFLAGS) $(OBJ) -std=c++98 -D NS=ft -o $@
			@ ./$@
			@ echo "====== Testing STD containers ======="
			@ $(CC) $(CFLAGS) $(OBJ) -std=c++98 -D NS=std -o $@
			@ ./$@

all_unit_tests: clean_unit_tests vector_test map_test stack_test set_test rb_tree_test

vector_test:
			@ echo "Vector unit test"
			@ $(CC) $(CFLAGS) unit_tests/vector.cpp -o $@ $(CRITERION)
			@ ./$@

map_test_0:	
			@ echo "Map_0 unit test"
			@ $(CC) $(CFLAGS) unit_tests/map.cpp -o $@ $(CRITERION)
			@ ./$@

map_test_1:	
			@ echo "Map_1 unit test"
			@ $(CC) $(CFLAGS) unit_tests/map2.cpp -o $@ $(CRITERION)
			@ ./$@

map_test:	clean_map_tests map_test_0 map_test_1

stack_test:	
			@ echo "Stack unit test"
			@ $(CC) $(CFLAGS) unit_tests/stack.cpp -o $@ $(CRITERION)
			@ ./$@

set_test:	
			@ echo "Set unit test"
			@ $(CC) $(CFLAGS) unit_tests/set.cpp -o $@ $(CRITERION)
			@ ./$@

rb_tree_test:
			@ echo "RbTree unit test"
			@ $(CC) $(CFLAGS) unit_tests/set.cpp -o $@ $(CRITERION)
			@ $(CC) $(CFLAGS) unit_tests/Rbtree.cpp -o $@ $(CRITERION)
			@ ./$@

%.o:		%.cpp $(HEADER)
			$(CC) $(CFLAGS) -std=c++98 -c $< -o $@

clean:
			rm -f $(OBJ)

clean_unit_tests:
			@ rm -f $(UNIT_TESTS)

clean_map_tests:
			@ rm -f map_test_0 map_test_1

fclean:		clean
			rm -f $(NAME) $(UNIT_TESTS)

re:			fclean all

.PHONY:		all clean fclean re $(UNIT_TESTS)
