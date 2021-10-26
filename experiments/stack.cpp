#include <stack.hpp>
#include <vector.hpp>
#include <iostream>

int main()
{
	ft::stack< int, ft::vector<int> > bad_stack;

	bad_stack.push(10000);
	std::cout << bad_stack.top() << std::endl;
}