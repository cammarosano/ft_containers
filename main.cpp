#include <iostream>
#include <string>
#include <deque>
#include <ctime>

#ifndef STL
# define STL 0
#endif

#if STL //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	#include <map.hpp>
	#include <stack.hpp>
	#include <vector.hpp>
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

void test_vector(void)
{
	std::cout << "Vector:" << std::endl;
	std::clock_t timer = std::clock();
	{

		ft::vector<std::string> vector_str;
		ft::vector<int> vector_int;
		ft::vector<Buffer> vector_buffer;


		// push_back()
		for (int i = 0; i < COUNT; i++)
		{
			vector_buffer.push_back(Buffer());
		}

		// operator[]
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer[idx].idx = 5;
		}

		// swap() with and empty vector
		ft::vector<Buffer>().swap(vector_buffer);

		// at()
		try
		{
			for (int i = 0; i < COUNT; i++)
			{
				const int idx = rand() % COUNT;
				vector_buffer.at(idx);
				std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
			}
		}
		catch(const std::exception& e)
		{
			//NORMAL ! :P
		}

		// iterators
		{
			for (int i = 0; i < 10000; i++)
				vector_int.push_back(rand());
			ft::vector<int>::iterator it = vector_int.begin();
			int sum = 0;
			while (it != vector_int.end())
				sum += *it++;
			std::cout << "should be constant with the same seed: " << sum << std::endl;
		}

		// capacity functions
		{
			vector_int.reserve(100);
			if (vector_int.capacity() == 100)
				std::cerr << "Error: Vector should not shrink !!" << std::endl;
			vector_int.reserve(20000);
			if (vector_int.capacity() != 20000)
				std::cerr << "Error: RESERVE FAILED!!" << std::endl;
			vector_int.resize(12000, 42);
			if (vector_int.size() != 12000)
				std::cerr << "Error: RESIZE FAILED!!" << std::endl;
			if (vector_int.empty())
				std::cerr << "Error: EMPTY!!" << std::endl;
		}

		// insert, assign, erase
		{
			ft::vector<int> copy(vector_int);
			copy.insert(copy.begin() + rand() % copy.size(), vector_int.begin(), vector_int.end());
			ft::vector<int>().swap(vector_int);
			vector_int.assign(copy.begin(), copy.end());
			vector_int.erase(vector_int.begin() + 1000, vector_int.end() - 1000);
		}

		// pop_back
		{
			int sum = 0;
			while (vector_int.size())
			{
				sum += vector_int.back();
				vector_int.pop_back();
			}
			std::cout << "should be constant with the same seed: " << sum << std::endl;
		}

		// relational operators
		{
			vector_int.assign(5, 42);
			ft::vector<int> copy(vector_int);
			if (!(vector_int == copy) || vector_int != copy)
				std::cerr << "Error: vector relational operator" << std::endl;
			vector_int.push_back(19);
			if (!(vector_int > copy))
				std::cerr << "Error: vector relational operator" << std::endl;
		}

	}
	std::cout << "Time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
}

void test_map(void)
{
	std::cout << "Map:" << std::endl;
	std::clock_t timer = std::clock();
	
	{
		ft::map<int, int> map_int;

		// insert()
		for (int i = 0; i < COUNT; ++i)
		{
			map_int.insert(ft::make_pair(rand(), rand()));
		}

		// operator[], lower_bound()
		int sum = 0;
		for (int i = 0; i < 10000; i++)
		{
			int access = map_int.lower_bound(rand())->first;
			sum += map_int[access];
		}
		std::cout << "should be constant with the same seed: " << sum << std::endl;

		// erase() , upper_bound(), size()
		for (int i = 0; i < 1000; i++)
		{
			ft::map<int,int>::iterator it = map_int.upper_bound(rand());
			map_int.erase(it);
		}
		std::cout << "should be constant with the same seed: " << map_int.size() << std::endl;

		// copy constructor, iterator, const iterator
		{
			ft::map<int, int> const copy = map_int;
			if (copy.size() != map_int.size())
				std::cerr << "Error: COPY MAP HAS DIFFERENT SIZE THAN SOURCE!!" << std::endl;
			ft::map<int, int>::iterator it = map_int.begin();
			ft::map<int, int>::const_iterator cit = copy.begin();
			while (it != map_int.end())
			{
				if (*it != *cit)
					std::cerr << "Error: MAPS ARE DIFFERENT!!" << std::endl;
				++it;
				cit++;
			}
		}

		// reverse_iterator
		{
			ft::map<int, int>::reverse_iterator rit, rit2;
			rit = map_int.rbegin();
			rit2 = rit;
			++rit;
			while (rit != map_int.rend())
			{
				if (map_int.key_comp()(rit2->first, rit->first))
					std::cerr << "Error: MAP MUST BE SORTED!!" << std::endl;
				++rit;
				++rit2;
			}
		}

		// count
		{
			for (int i = 0; i < 100; i++)
				map_int[i * 1000] = 0;
			int sum = 0;
			for (int i = 0; i < 100000; i++)
				sum += map_int.count(i);
			std::cout << "should be constant with the same seed: " << sum << std::endl;
		}

	}
	std::cout << "Time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
}

void test_stack(void)
{
	std::cout << "Stack:" << std::endl;
	std::clock_t timer = std::clock();

	{
		ft::stack<int> stack_int;
		// ft::stack<Buffer, std::deque<int> > stack_deq_buffer; // wrong type for undelying container
		MutantStack<char> iterable_stack;

		// push()
		for (char letter = 'a'; letter <= 'z'; letter++)
			iterable_stack.push(letter);

		// vector (underlying container) iterator
		for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
		{
			std::cout << *it;
		}
		std::cout << std::endl;
	}
	std::cout << "Time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
}

void test_map_clear()
{
	ft::map<int,int> m;
	std::clock_t timer = std::clock();

	for (int i = 0; i < 2000000; i++)
		m[rand()] = rand();
	std::cout << "Insertion time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
				"s" << std::endl;
	
	m.clear();
	std::cout << "Clear time: " << (std::clock() - timer) / (float)CLOCKS_PER_SEC <<
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

	// test_vector();
	// test_map();
	// test_stack();
	test_map_clear();

	return (0);
}
