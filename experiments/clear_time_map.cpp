#include <iostream>
#include <ctime>
#include <map.hpp>

void test_map_clear()
{
	ft::map<int,int> m;
	std::clock_t timer = std::clock();

	for (int i = 0; i < 2000000; i++)
		m[rand()] = rand();
	std::cout << "Insertion time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
	
	timer = std::clock();
	m.clear();
	std::cout << "Clear time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
}