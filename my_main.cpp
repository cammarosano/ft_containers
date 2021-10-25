#include "vector.hpp"
#include <vector>
#include <ctime>
#include <iostream>
#ifndef NS
 # define NS ft
#endif

int main()
{
	std::cout << "****** TESTING" << " NS " << "******" << std::endl;
	NS::vector<int> v;
	std::clock_t t;

	t = std::clock();
	{
		int i = -1;
		while (++i < 10000)
		{
			v.push_back(i);
			// std::cout	<< "size: " << v.size() << '\n'
						// << "capacity: " << v.capacity() << '\n';
		}
	}
	t = clock() - t;
	std::cout << "push_back: " << t << std::endl;

	t = std::clock();
	{
		while (v.size())
			v.pop_back();
	}
	t = clock() - t;
	std::cout << "pop_back: " << t << std::endl;
}