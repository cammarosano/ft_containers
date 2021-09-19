#include "ft_vector.hpp"
#include <iostream>

int main()
{
	ft_vector<int> v;
	int	*p;

	std::cout << "size: " << v.size() << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;

	for (int i = 0; i < 100; i++)
	{
		v.push_back(i);
		std::cout << "size: " << v.size() << std::endl;
		std::cout << "capacity: " << v.capacity() << std::endl;
	}

	std::cout << "vector content: " << std::endl;
	p = v.begin();
	for (size_t i = 0; i < v.size(); i++)
		std::cout << p[i] << " ";
	std::cout << std::endl;

}