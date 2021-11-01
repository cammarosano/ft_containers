#include <iostream>
#include <string>
#include <deque>
#include <ctime>

#ifndef STL
# define STL 0
#endif

#if STL //CREATE A REAL STL EXAMPLE
	#include <set>
	namespace ft = std;
#else
	#include <set.hpp>
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096

#define COUNT (MAX_RAM / BUFFER_SIZE)


void test_set(void)
{
	std::cout << "Set:" << std::endl;
	std::clock_t timer = std::clock();
	
	{
		ft::set<int> set_int;

		// insert()
		for (int i = 0; i < COUNT; ++i)
		{
			set_int.insert(rand());
		}

		// lower_bound()
		int sum = 0;
		for (int i = 0; i < 10000; i++)
		{
			sum += *set_int.lower_bound(rand());
		}
		std::cout << "should be constant with the same seed: " << sum << std::endl;

		// erase() , upper_bound(), size()
		for (int i = 0; i < 1000; i++)
		{
			ft::set<int>::iterator it = set_int.upper_bound(rand());
			set_int.erase(it);
		}
		std::cout << "should be constant with the same seed: " << set_int.size() << std::endl;

		// copy constructor, iterator, const iterator
		{
			ft::set<int> const copy(set_int);
			if (copy.size() != set_int.size())
				std::cerr << "Error: COPY SET HAS DIFFERENT SIZE THAN SOURCE!!" << std::endl;
			ft::set<int>::iterator it = set_int.begin();
			ft::set<int>::const_iterator cit = copy.begin();
			while (it != set_int.end())
			{
				if (*it != *cit)
					std::cerr << "Error: SETS ARE DIFFERENT!!" << std::endl;
				++it;
				cit++;
			}
		}

		// reverse_iterator
		{
			ft::set<int>::reverse_iterator rit, rit2;
			rit = set_int.rbegin();
			rit2 = rit;
			++rit;
			while (rit != set_int.rend())
			{
				if (set_int.key_comp()(*rit2, *rit))
					std::cerr << "Error: SET MUST BE SORTED!!" << std::endl;
				++rit;
				++rit2;
			}
		}

		// count
		{
			for (int i = 0; i < 100; i++)
				set_int.insert(i * 1000);
			int sum = 0;
			for (int i = 0; i < 100000; i++)
				sum += set_int.count(i);
			std::cout << "should be constant with the same seed: " << sum << std::endl;
		}

	}
	std::cout << "Time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
}

int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	if (STL)
		std::cout	<< "\033[32m" << "Testing STL containers:"
					<< "\033[0m" << std::endl;
	else
		std::cout	<< "\033[36m" << "Testing FT containers:"
					<< "\033[0m" << std::endl;
	const int seed = atoi(argv[1]);
	srand(seed);

	test_set();

	return (0);
}
