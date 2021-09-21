#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include <iostream> // debug messages
# include "iterator.hpp"
# include <stdexcept>

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
	typedef size_t						size_type;
	typedef value_type &				reference;
	typedef value_type const &			const_reference;
	typedef value_type *				pointer;
	typedef value_type const *			const_pointer;

private:
	value_type *		_array;
	size_type			_size;
	size_type			_capacity;
	std::allocator<value_type>	_allocator;

	void increase_capacity(void);

public:
	// TODO: default constructor, copy constructor, =op overload
	// other constructors



	vector(): _array(0), _size(0), _capacity(0)
	{
		// _array is initialized to a NULL pointer
	} 

	~vector()
	{
		if (_capacity)
		{
			for (size_type i = 0; i < _size; i++)
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

	/* Capacity: */

	size_type size(void) const
	{
		return (_size);
	}

	size_type max_size() const
	{
		return (_allocator.max_size());
	}

	// TODO : resize

	size_type capacity(void) const
	{
		return (_capacity);
	}

	/* Iterators */

	iterator begin(void)
	{
		return (iterator(_array));
	}

	iterator end(void)
	{
		return (iterator(_array + _size));
	}

	/* Element access */

	reference operator[](size_type n)
	{
		return (_array[n]);
	}
	const_reference operator[](size_type n) const
	{
		return (_array[n]);
	}

	reference at(size_type n)
	{
		if (n >= _size)
			throw (std::out_of_range("ft_vector: out of range"));
		return (_array[n]);
	}
	const_reference at(size_type n) const
	{
		if (n >= _size)
			throw (std::out_of_range("ft_vector: out of range"));
		return (_array[n]);
	}

	reference front(void)
	{
		return (_array[0]);
	}
	const_reference front(void) const
	{
		return (_array[0]);
	}

	reference back()
	{
		return (_array[_size - 1]);
	}
	const_reference back() const
	{
		return (_array[_size - 1]);
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