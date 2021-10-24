#include <iostream>
#include <vector>
#include "../ft_vector.hpp"

int main()
{
	ft::vector<int> my_v(5, 42);
	std::vector<int> stl_v(10, 19);

	// try
	// {
	// 	stl_v.assign(my_v.end(), my_v.begin());
	// }
	// catch(const std::length_error& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	

	std::vector<int>::iterator it = stl_v.begin();
	// while (it != stl_v.end())
		// std::cout << *it++ << " ";
	// std::cout << std::endl;



	int * ptr = it.base();
	std::cout << ptr << std::endl;
}