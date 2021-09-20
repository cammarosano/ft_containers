#include "ft_vector.hpp"
#include <iostream>
#include <algorithm>

int main()
{
	ft::vector<int> v;

	std::cout << "size: " << v.size() << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;

	int n = 16;
	for (int i = 0; i < n; i++)
	{
		v.push_back(n - i);
		std::cout << "size: " << v.size() << std::endl;
		std::cout << "capacity: " << v.capacity() << std::endl;
	}

	std::cout << "vector content: " << std::endl;
	ft::vector<int>::iterator it = v.begin();
	ft::vector<int>::iterator ite = v.end();
	while (it != ite)
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;

	// testing if my iterators work with algo lib
	std::sort(v.begin(), v.end());

	std::cout << "vector content after sorting: " << std::endl;
	it = v.begin();
	ite = v.end();
	while (it != ite)
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;
}
