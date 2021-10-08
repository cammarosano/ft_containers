#ifndef FT_IS_INTEGRAL_HPP
# define FT_IS_INTEGRAL_HPP

namespace ft
{
	/* is_integral */

	struct intTrue {static bool const value = true;};

	template <typename T>
	struct is_integral {static bool const value = false;};

	template <>	struct is_integral<bool> : intTrue {}; // struct has public inheritance by default
	template <>	struct is_integral<char> : intTrue {};
	template <>	struct is_integral<unsigned char> : intTrue {};
	template <>	struct is_integral<signed char> : intTrue {};
	// template <>	struct is_integral<char16_t> : intTrue {};
	// template <>	struct is_integral<char32_t> : intTrue {};
	template <>	struct is_integral<wchar_t> : intTrue {};
	template <>	struct is_integral<short> : intTrue {};
	template <>	struct is_integral<unsigned short> : intTrue {};
	template <>	struct is_integral<int> : intTrue {};
	template <>	struct is_integral<unsigned int> : intTrue {};
	template <>	struct is_integral<long> : intTrue {};
	template <>	struct is_integral<unsigned long> : intTrue {};
	template <>	struct is_integral<long long> : intTrue {};
	template <>	struct is_integral<unsigned long long> : intTrue {};
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

	/* make_pair */
	template <typename T1, typename T2>
	pair<T1, T2> make_pair(T1 x, T2 y)
	{
		return (pair<T1, T2>(x, y));
	}


	/* Node */

	template < typename ValueType >
	struct Node
	{
		Node *		left;
		Node *		right;
		Node *		parent;
		ValueType *	content;

		Node(ValueType *content):
		left(NULL), right(NULL), parent(NULL), content(content)
		{
		}
		// default contructor - should not be used
		Node():
		left(NULL), right(NULL), parent(NULL), content(NULL)
		{
		}
		~Node()
		{
		}
	};

	template <typename T>
	struct less
	{
		bool operator()(T const & x, T const & y) const
		{
			return (x < y);
		}
	};

}



#endif