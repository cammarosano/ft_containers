#ifndef FT_IS_INTEGRAL_HPP
# define FT_IS_INTEGRAL_HPP

#include <iostream> // debug only

namespace ft
{
	struct true_type {static bool const value = true;};

	/* is_integral */

	template <typename T>
	struct is_integral {static bool const value = false;};

	template <>	struct is_integral<bool> : true_type {}; // struct has public inheritance by default
	template <>	struct is_integral<char> : true_type {};
	template <>	struct is_integral<unsigned char> : true_type {};
	template <>	struct is_integral<signed char> : true_type {};
	// template <>	struct is_integral<char16_t> : true_type {};
	// template <>	struct is_integral<char32_t> : true_type {};
	template <>	struct is_integral<wchar_t> : true_type {};
	template <>	struct is_integral<short> : true_type {};
	template <>	struct is_integral<unsigned short> : true_type {};
	template <>	struct is_integral<int> : true_type {};
	template <>	struct is_integral<unsigned int> : true_type {};
	template <>	struct is_integral<long> : true_type {};
	template <>	struct is_integral<unsigned long> : true_type {};
	template <>	struct is_integral<long long> : true_type {};
	template <>	struct is_integral<unsigned long long> : true_type {};
	// TODO: const and volatile ?


	/* enable_if */

	template<bool Cond, typename T = void>
	struct enable_if {};// "type" is not defined
	template<typename T>
	struct enable_if<true, T> {typedef T type;}; // template specialization


	/* equal */ 
	template < typename It_1, typename It_2 >
	bool equal (It_1 first1, It_1 last1, It_2 first2)
	{
		while (first1 != last1)
		{
			if (!(*first1 == *first2))
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}

	// TODO: predicate version


	/* lexigographical_compare */

	//return true if first range is lexicogr smaller than second range
	template < typename It_1, typename It_2 >
	bool lexicographical_compare(It_1 first1, It_1 last1,
									It_2 first2, It_2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return (false);
			if (*first1 < *first2)
				return (true);
			// at this point, *first1 == *first2, loop goes on
			++first1;
			++first2;
		}
		// at this point, first range reached the end
		return (first2 != last2);
	}


	/* class pair */
	template < typename T1, typename T2 >
	struct pair
	{
		T1 first;
		T2 second;

		pair():  first(T1()), second(T2()) // is this value-initialization ?
		{
			// std::cout << "pair default constructor\n";
		}

		pair(T1 const & a, T2 const & b): first(a), second(b)
		{
			// std::cout << "pair initialization constructor\n";
		}

		template<typename U, typename V> pair(pair<U,V> const & pr):
		first(pr.first), second(pr.second)
		{
		}

	// raises deprecated error. It compiles with std=c++98 (reference says this is implicit declared)
		// pair & operator=(pair const & rhs)
		// {
		// 	first = rhs.first;
		// 	second = rhs.second;
		// 	return (*this);
		// }

		// enable convertion from pair<T1, T2> to pair<const T1, T2>
		// operator pair<const T1, T2>() const
		// {
		// 	return (pair<const T1, T2>(first, second));
		// }
	};

	/* Relational operators(pair) */

	template <typename T1, typename T2>
	bool operator==(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}

	template <typename T1, typename T2>
	bool operator!=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (!(lhs == rhs));
	}

	template <typename T1, typename T2>
	bool operator<(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
	}

	template <typename T1, typename T2>
	bool operator<=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (!(rhs < lhs));
	}

	template <typename T1, typename T2>
	bool operator>(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (rhs < lhs);
	}

	template <typename T1, typename T2>
	bool operator>=(pair<T1, T2> const & lhs, pair<T1, T2> const & rhs)
	{
		return (!(lhs < rhs));
	}


	/* make_pair */
	template <typename T1, typename T2>
	pair<T1, T2> make_pair(T1 x, T2 y)
	{
		return (pair<T1, T2>(x, y));
	}


	/* class less */

	template <typename T>
	struct less
	{
		bool operator()(T const & x, T const & y) const
		{
			return (x < y);
		}
	};

	/*	 Iterator_traits  */

	template <typename Iterator>
	class iterator_traits
	{
	public:
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	// my implementation of std::distance

	namespace distance_dispatcher
	{

	template<typename It, typename Tag>
	typename ft::iterator_traits<It>::difference_type
	distance(It first, It last, Tag)
	{
		// std::cout << "not a RA iterator...\n";
		typename ft::iterator_traits<It>::difference_type n;

		n = 0;
		while (first != last)
		{
			++n;
			++first;
		}
		// std::cout << "distance is " << n << std::endl;
		return (n);
	}

	// specialization (overload) of distance for Random Access iterator
	template<typename RAiterator>
	typename ft::iterator_traits<RAiterator>::difference_type
	distance(RAiterator first, RAiterator last, std::random_access_iterator_tag)
	{
		// std::cout << "RA iterator!!!\n";
		return (last - first);
	}

	} // namespace distance_dipatcher

	template<typename InputIterator>
	typename ft::iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last)
	{
		return (distance_dispatcher::distance(first, last, 
			typename ft::iterator_traits<InputIterator>::iterator_category()));
	}




} // namespace ft


#endif