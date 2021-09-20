#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include <iostream> // debug messages

namespace ft
{
	template <typename T> class vector;
	
} // namespace ft


template <typename T>
class ft::vector
{
	typedef T value_type;

public:
	class iterator
	{
	private:
		value_type *	ptr;
	public:

		// iterator_traits: (alternatively, write a template specialization of std::iterator_traits for this type?)
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef T *								pointer;
		typedef T &								reference;
		typedef std::random_access_iterator_tag	iterator_category;

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
		iterator operator+(int n) const // maybe a template for any integer type?
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
	

private:
	value_type *		_array;
	unsigned int		_size;
	unsigned int		_capacity;
	std::allocator<value_type>	_allocator;

	void increase_capacity(void);

public:
	vector(): _array(0), _size(0), _capacity(0)
	{
		// _array is initialized to a NULL pointer
	} 

	~vector()
	{
		if (_capacity)
		{
			for (unsigned int i = 0; i < _size; i++)
				_allocator.destroy(&_array[i]);
			_allocator.deallocate(_array, _capacity);
		}
	}

	void push_back(value_type const & val)
	{
		if (_size == _capacity)
			increase_capacity();
		_allocator.construct(&_array[_size], val);
		_size += 1;
	}	

	unsigned int size(void) const
	{
		return (_size);
	}

	unsigned int capacity(void) const
	{
		return (_capacity);
	}

	iterator begin(void)
	{
		return (iterator(_array));
	}

	iterator end(void)
	{
		return (iterator(_array + _size));
	}
};

template <typename T>
void ft::vector<T>::increase_capacity(void)
{
	if (_capacity == 0)
	{
		_array = _allocator.allocate(1);
		_capacity = 1;
		return ;
	}
	T * new_array = _allocator.allocate(_capacity * 2);

	for (unsigned int i = 0; i < _size; i++)
	{
		_allocator.construct(&new_array[i], _array[i]);
		_allocator.destroy(&_array[i]);
	}
	_allocator.deallocate(_array, _capacity);
	_capacity *= 2;
	_array = new_array;
}


#endif