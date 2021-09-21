#include "ft_vector.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

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

	std::cout << v[3] << std::endl;

	try
	{
		std::cout << v.at(23) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << "front(): " << v.front() << std::endl;
	std::cout << "back(): " << v.back() << std::endl;

	// ft::vector<int> vec2;
	// std::cout << "back(): " << vec2.back() << std::endl;

	std::vector<int> vec2;
	std::cout << vec2.get_allocator().max_size() << std::endl;
	std::cout << vec2.max_size() << std::endl;



}
