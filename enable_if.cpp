#include <iostream>
#include <iterator>
#include <vector>

namespace ft
{
	template<bool Cond, typename T>
	struct enable_if
	{
		// "type" is not defined
	};

	template<typename T>
	struct enable_if<true, T>  // template specialization
	{
		typedef T type;
	};
}

template <typename T>
struct is_int {static bool const value = false;};
template <>
struct is_int<int> {static bool const value = true;};

template <typename T>
struct is_iter {static bool const value = false;};
template <>
struct is_iter<std::random_access_iterator_tag> {static bool const value = true;};



template <typename T>
void foo(typename ft::enable_if<is_int<T>::value, T>::type arg, T arg2)
{
	std::cout << "I am enabled!\n";
	std::cout << arg << std::endl;
	std::cout << arg2 << std::endl;
}

template <typename T>
void foo(typename ft::enable_if<is_iter<typename std::iterator_traits<T>::iterator_category >::value, T>::type arg,
			T arg2)
{
	std::cout << "Random Access Iterator version is enabled\n";
	while (arg != arg2)
		std::cout << *arg++ << " ";
	std::cout << std::endl;

}

template < typename T >
struct is_integral {static bool const value = false;};
template <>
struct is_integral<int> {static bool const value = true;};


#include "ft_is_integral.hpp"
int main()
{
	// foo(2, 3);

	// std::vector<int> v;
	// v.push_back(1);
	// v.push_back(2);
	// v.push_back(3);
	// foo(v.begin(), v.end());

	std::cout << std::boolalpha;
	// std::cout << is_integral<long>::value << "\n";


	std::cout << ft::is_integral<unsigned char>::value << "\n";
}

