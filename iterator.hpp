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
			// std::cout << "iterator default constructor\n";
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
		iterator operator+(difference_type n) const 
		{
			return (iterator(ptr + n));
		}
		iterator operator-(difference_type n) const 
		{
			return (iterator(ptr - n));
		}
		difference_type operator-(iterator const & rhs) const
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
		iterator & operator+=(difference_type n)
		{
			ptr = ptr + n;
			return (*this);
		}	
		iterator & operator-=(difference_type n)
		{
			ptr = ptr - n;
			return (*this);
		}	
		value_type & operator[](difference_type i)
		{
			return (ptr[i]);
		}
		pointer	operator->() const
		{
			return (ptr);
		}
};

template <typename T>
ft::iterator<T> operator+(typename ft::iterator<T>::difference_type n,
							ft::iterator<T> it)
{
	return (it + n);
}

#endif