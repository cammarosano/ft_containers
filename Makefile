SRC = main.cpp
SRC_BONUS = main_bonus.cpp
CONTAINERS = map.hpp set.hpp stack.hpp vector.hpp
HEADERS = Rbtree.hpp reverse_iterator.hpp tree_iterator.hpp utils.hpp vector_iterator.hpp
HEADER = $(addprefix containers/,$(CONTAINERS)) $(addprefix includes/,$(HEADERS))
INCLUDE = -I includes -I containers
CRITERION = -l criterion -I ~/.brew/include -L ~/.brew/lib -std=c++11
CC = clang++
CFLAGS = -Wall -Wextra -Werror $(INCLUDE) -g -fsanitize=address 
NAME = test_ft
UNIT_TESTS = vector_test map_test_0 map_test_1 rb_tree_test set_test stack_test
all:		$(NAME) test_stl

$(NAME):	$(SRC) $(HEADER)
			$(CC) $(CFLAGS) -std=c++98 -D STL=0 -o $@ $(SRC) 

test_stl:	$(SRC)
			$(CC) $(CFLAGS) -std=c++98 -D STL=1 -o $@ $^ 

bonus:		bonus_test_ft bonus_test_stl

bonus_test_ft:	$(SRC_BONUS) $(HEADER)
			$(CC) $(CFLAGS) -std=c++98 -D STL=0 -o $@ $(SRC_BONUS) 

bonus_test_stl:	$(SRC_BONUS)
			$(CC) $(CFLAGS) -std=c++98 -D STL=1 -o $@ $^ 

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
			@ $(CC) $(CFLAGS) unit_tests/Rbtree.cpp -o $@ $(CRITERION)
			@ ./$@

%.o:		%.cpp 
			$(CC) $(CFLAGS) -std=c++98 -c $< -o $@

clean:
			rm -rf $(OBJ) *.dSYM

clean_unit_tests:
			@ rm -f $(UNIT_TESTS)

clean_map_tests:
			@ rm -f map_test_0 map_test_1

fclean:		clean clean_unit_tests
			rm -f $(NAME) test_stl bonus_test_ft bonus_test_stl

re:			fclean all

.PHONY:		all clean fclean re $(UNIT_TESTS)
