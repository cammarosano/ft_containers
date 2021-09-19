#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>

template <typename T>
class ft_vector
{
	typedef T value_type;

private:
	value_type *		_array;
	unsigned int		_size;
	unsigned int		_capacity;
	std::allocator<value_type>	_allocator;

	void increase_capacity(void)
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

public:
	ft_vector(): _array(nullptr), _size(0), _capacity(0)
	{
	} 

	~ft_vector()
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

	// shall be changed to an iterator
	T * begin(void)
	{
		return (_array);
	}
};


#endif