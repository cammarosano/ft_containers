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





}



#endif