/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:38:05 by rcammaro          #+#    #+#             */
/*   Updated: 2021/11/02 19:44:52y rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IS_INTEGRAL_HPP
# define FT_IS_INTEGRAL_HPP

#include <iostream> // debug only

namespace ft
{
	/* true and false types */

	template <typename T, T v>
	struct integral_constant
	{
		static const T value = v;
		typedef T value_type;
		typedef integral_constant<T,v> type;
		operator T() const {return v;}
	};

	typedef integral_constant<bool, true>	true_type;
	typedef integral_constant<bool, false>	false_type;

	/* remove_cv */

	template <typename T> struct remove_cv 				{ typedef T type; };
	template <typename T> struct remove_cv<const T>		{ typedef T type; };
	template <typename T> struct remove_cv<volatile T>	{ typedef T type; };
	template <typename T>
	struct remove_cv<const volatile T>					{ typedef T type; };

	/* is_integral */

	template <typename T> struct is_int : false_type {};

	template <>	struct is_int<bool> : true_type {}; 
	template <>	struct is_int<char> : true_type {};
	template <>	struct is_int<wchar_t> : true_type {};
	template <>	struct is_int<signed char> : true_type {};
	template <>	struct is_int<short int> : true_type {};
	template <>	struct is_int<int> : true_type {};
	template <>	struct is_int<long int> : true_type {};
	template <>	struct is_int<long long int> : true_type {};
	template <>	struct is_int<unsigned char> : true_type {};
	template <>	struct is_int<unsigned short int> : true_type {};
	template <>	struct is_int<unsigned int> : true_type {};
	template <>	struct is_int<unsigned long int> : true_type {};
	template <>	struct is_int<unsigned long long int> : true_type {};

	template <typename T>
	struct is_integral : is_int<typename remove_cv<T>::type> {};

	/* is_same */

	template <typename T, typename U>
	struct is_same: false_type {}; 

	template <typename T>
	struct is_same<T, T> : true_type {};

	/* static_assert c++98 */
	
	template <bool expr>
	struct _static_assert {};

	template <>
	struct _static_assert<true>
	{
		static char const test;
	};

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

	template < typename It_1, typename It_2, typename P >
	bool equal (It_1 first1, It_1 last1, It_2 first2, P pred)
	{
		while (first1 != last1)
		{
			if (!pred(*first1, *first2))
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}


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

	template < typename It_1, typename It_2, typename C >
	bool lexicographical_compare(It_1 first1, It_1 last1,
									It_2 first2, It_2 last2, C comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1))
				return (false);
			if (comp(*first1, *first2))
				return (true);
			++first1;
			++first2;
		}
		return (first2 != last2);
	}

	/* class pair */
	
	template < typename T1, typename T2 >
	struct pair
	{
		typedef T1	first_type;
		typedef T2	second_type;

		first_type	first;
		second_type	second;

		pair():  first(first_type()), second(second_type())
		{
		}

		template<typename U, typename V>
		pair(pair<U,V> const & pr):
		first(pr.first), second(pr.second)
		{
		}

		pair(first_type const & a, second_type const & b): first(a), second(b)
		{
		}
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

	/* distance */

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