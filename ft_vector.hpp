#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include <iostream> // debug messages
# include "iterator.hpp"
# include <stdexcept>
# include <iterator> // iterator_traits
# include <typeinfo> // type_info
# include "ft_is_integral.hpp"

namespace ft
{
	template <typename T> class vector;

	template<typename InputIterator>
	typename std::iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last);

	// enable_if
	template<bool Cond, typename T>
	struct enable_if {};// "type" is not defined
	template<typename T>
	struct enable_if<true, T> {typedef T type;}; // template specialization
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
	std::allocator<T>	_allocator;

	void reallocate(size_type new_capacity);

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

	/* Capacity: */

	size_type size(void) const
	{
		return (_size);
	}

	size_type max_size() const
	{
		return (_allocator.max_size());
	}

	void resize(size_type n, value_type val = value_type())
	{
		if (n < _size)
		{
			for (size_type i = n; i < _size; ++i)
				_allocator.destroy(&_array[i]);
		}
		else
		{
			if (n > _capacity)
				reallocate(n);
			for (size_type i = _size; i < n; ++i)
				_allocator.construct(&_array[i], val);
		}
		_size = n;
	}

	size_type capacity(void) const
	{
		return (_capacity);
	}

	bool empty(void) const
	{
		return (_size == 0);
	}

	void reserve(size_type n)
	{
		if (n > _capacity)
		{
			if (n > max_size())
				throw std::length_error("ft_vector: length error");
			reallocate(n);
		}
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
	
	// TODO rbegin, rend 


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
	
	/* Modifiers */

	void assign(size_type n, value_type const & val)
	{
		// destroy original elems
		for (size_type i = 0; i < _size; i++)
			_allocator.destroy(&_array[i]);
		
		// increase capacity if needed (deallocate and allocate)
		if (n > _capacity)
		{
			if (_array)
				_allocator.deallocate(_array, _capacity);
			_array = _allocator.allocate(n);
			_capacity = n;
		}

		// construct new elems
		for (size_type i = 0; i < n; i++)
			_allocator.construct(&_array[i], val);

		// update _size
		_size = n;
	}

	//  watch out for self assignation?? check reference (I think it's undefined behaviour)
	template < typename InputIterator >
	void assign(
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type first,
		InputIterator last)
	{
		// destroy original elems
		for (size_type i = 0; i < _size; i++)
			_allocator.destroy(&_array[i]);
		_size = 0;
		
		// if iterator is not at least forward_iterator, push one by one
		if (typeid(typename std::iterator_traits<InputIterator>::iterator_category) == typeid(std::input_iterator_tag))
		{
			std::cout << "Iterator has type input_iterator\n";
			while (first != last)
				push_back(*first++);
		}
		else // should I test for fwd, bidir or rand_acc iterator??
		{
			size_type n = ft::distance(first, last);

			if (n > _capacity)
			{
				if (_array)
					_allocator.deallocate(_array, _capacity);
				_array = _allocator.allocate(n);
				_capacity = n;
			}
			while (first != last)
			{
				_allocator.construct(&_array[_size], *first);
				++_size;
				++first;
			}
		}
	}

	void push_back(value_type const & val)
	{
		// reallocate if necessary
		if (_size == 0)
			reallocate(1);
		else if (_size == _capacity)
			reallocate(2 * _capacity);

		// construct new element at the end
		_allocator.construct(&_array[_size], val);
		_size += 1;
	}	

	void pop_back(void)
	{
		if (_size == 0) // STL vector has undefined behavior for popping an empty container
			return ;
		_allocator.destroy(&_array[_size - 1]);
		_size -= 1;
	}


	
};


template <typename T>
void ft::vector<T>::reallocate(size_type new_capacity)
{
	if (!(new_capacity > _capacity))
		return ;

	// allocate new array
	T * new_array = _allocator.allocate(new_capacity);

	// copy elements to new array and destroy them
	for (size_type i = 0; i < _size; i++)
	{
		_allocator.construct(&new_array[i], _array[i]);
		_allocator.destroy(&_array[i]);
	}
	// deallocate old array
	if (_array)
		_allocator.deallocate(_array, _capacity);
	// update _capacity and _array
	_capacity = new_capacity;
	_array = new_array;
}


// my implementation of std::distance

template<typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type
ft::distance(InputIterator first, InputIterator last)
{
	if (typeid(typename std::iterator_traits<InputIterator>::iterator_category)
		== typeid(std::random_access_iterator_tag))
	{
		std::cout << "Iterator of type random-access\n";
		return (last - first);
	}
	
	typename std::iterator_traits<InputIterator>::difference_type n;

	n = 0;
	while (first != last)
	{
		++n;
		++first;
	}
	return (n);
}


#endif