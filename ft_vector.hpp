#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include <iostream> // debug messages
# include "iterator.hpp"

namespace ft
{
	template <typename T> class vector;
}

template <typename T>
class ft::vector
{
public:
	typedef T							value_type;
	typedef typename ft::iterator<T>	iterator;

private:
	value_type *		_array;
	unsigned int		_size;
	unsigned int		_capacity;
	std::allocator<value_type>	_allocator;

	void increase_capacity(void);

public:
	// TODO: default constructor, copy constructor, =op overload



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

	value_type & operator[](size_t n)
	{
		return (_array[n]);
	}
	value_type const & operator[](size_t n) const
	{
		return (_array[n]);
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