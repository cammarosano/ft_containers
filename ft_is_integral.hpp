#ifndef FT_IS_INTEGRAL_HPP
# define FT_IS_INTEGRAL_HPP

namespace ft
{
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
	
}

// TODO: const and volatile ?


#endif