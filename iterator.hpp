#ifndef ITERATOR_HPP
# define ITERATOR_HPP

namespace ft
{
	template <typename T> class iterator;
}

template <typename T>
class ft::iterator
{
public:
	// iterator_traits: (alternatively, write a template specialization of std::iterator_traits for this type?)
	typedef std::ptrdiff_t					difference_type;
	typedef T								value_type;
	typedef T *								pointer;
	typedef T &								reference;
	typedef std::random_access_iterator_tag	iterator_category;

private:
	T *	ptr;

public:
		iterator()
		{
			std::cout << "iterator default constructor\n";
		}
		iterator(value_type *address): ptr(address)
		{
			// std::cout << "iterator parameter constructor\n";
		}
		iterator(iterator const & src): ptr(src.ptr) // copy constructor
		{
		}
		~iterator()
		{
		}
		iterator & operator=(iterator const & rhs)
		{
			ptr = rhs.ptr;
			return (*this);
		}
		value_type & operator*() // const?
		{
			return (*ptr);
		}
		iterator & operator++()	// prefix increment
		{
			ptr += 1; // pointer arithmetic must be supported by value_type *
			return (*this);
		}
		iterator operator++(int) // postfix increment (returns a new object)
		{
			iterator temp(*this);

			ptr += 1;
			return (temp);
		}
		iterator & operator--()
		{
			ptr -= 1;
			return (*this);
		}
		iterator operator--(int)
		{
			iterator temp(*this);

			ptr -= 1;
			return (temp);
		}
		iterator operator+(int n) const // maybe a template for any integer type? OR JUST USE difference_type
		{
			return (iterator(ptr + n));
			// TODO: non-member function overload for the inverser order of operands
		}
		iterator operator-(int n) const // maybe a template for any integer type?
		{
			return (iterator(ptr - n));
			// TODO: non-member function overload for the inverser order of operands
		}
		difference_type operator-(iterator const & rhs)
		{
			return (ptr - rhs.ptr);
		}
		bool operator==(iterator const & rhs) const
		{
			return (ptr == rhs.ptr);
		}
		bool operator!=(iterator const & rhs) const
		{
			return (ptr != rhs.ptr);
		}
		bool operator>(iterator const & rhs) const
		{
			return (ptr > rhs.ptr);
		}
		bool operator<(iterator const & rhs) const
		{
			return (ptr < rhs.ptr);
		}
		bool operator>=(iterator const & rhs) const
		{
			return (ptr >= rhs.ptr);
		}
		bool operator<=(iterator const & rhs) const
		{
			return (ptr <= rhs.ptr);
		}
		iterator & operator+=(int n) // not sure about the int
		{
			ptr = ptr + n;
			return (*this);
		}	
		iterator & operator-=(int n) // not sure about the int
		{
			ptr = ptr - n;
			return (*this);
		}	
		value_type & operator[](int i) // or size_t ? 
		{
			return (ptr[i]);
		}
};

#endif