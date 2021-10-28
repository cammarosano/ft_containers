#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include <memory>
# include <iostream> // debug messages
# include "vector_iterator.hpp"
# include "reverse_iterator.hpp"
# include <stdexcept>
# include <iterator> // iterator_tags
# include "utils.hpp"

namespace ft
{

template <typename T>
class vector
{
public:
	typedef T										value_type;
	typedef std::allocator<T>						allocator_type;
	typedef value_type &							reference;
	typedef value_type const &						const_reference;
	typedef value_type *							pointer;
	typedef value_type const *						const_pointer;
	typedef ft::vector_iterator<T>					iterator;
	typedef ft::vector_iterator<T const>			const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef typename iterator::difference_type		difference_type;
	typedef size_t									size_type;

private:
	pointer				_array;
	size_type			_size;
	size_type			_capacity;
	allocator_type		_allocator;

	/* ------------ Implementation detais ----------------- */

	void reallocate(size_type new_capacity)
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

	// dst points to allocated unitialized memory
	void move_to_new_block(pointer dst, pointer src, size_type n)
	{
		while (n--)
		{
			_allocator.construct(dst++, *src);
			_allocator.destroy(src++);
		}
	}

	// capacity must be >= _size + shift
	void shift_right(pointer src, size_type shift)
	{
		pointer dst = src + shift;
		size_type n = _array + _size - src;  // number of elements to be moved
		while (n--)
		{
			_allocator.construct(dst + n, src[n]);
			_allocator.destroy(src + n);
		}
	}

	// returns pointer to insertion position
	pointer reallocate4insertion(pointer src, size_type shift)
	{
		size_type	new_capacity, n_1st_part;
		pointer		new_array, insert_pos;

		// allocate new array
		new_capacity = _capacity * 2;
		if (new_capacity < _size + shift)
			new_capacity = _size + shift;
		new_array = _allocator.allocate(new_capacity);

		// move first part
		n_1st_part = src - _array;
		move_to_new_block(new_array, _array, n_1st_part);

		// move second part
		insert_pos = new_array + n_1st_part;
		move_to_new_block(insert_pos + shift, _array + n_1st_part,
							_size - n_1st_part);

		// deallocate old array
		_allocator.deallocate(_array, _capacity);
		_array = new_array;

		// update capacity
		_capacity = new_capacity;
		return (insert_pos);
	}

	// Returns pointer to begining of insertion position
	pointer make_room4insertion(iterator it_position, size_type n)
	{
		pointer p_position;
		
		p_position = _array + (it_position - begin()); // avoiding to dereference it to end
		if (_size + n <= _capacity) // shift elements by n (no realloc)
			shift_right(p_position, n);
		else
			p_position = reallocate4insertion(p_position, n);
		_size += n;
		return (p_position);
	}


	template <typename InputIt>
	void assign_dispatcher(InputIt first, InputIt last, std::input_iterator_tag)
	{
		clear();
		
		while (first != last)
			push_back(*first++);
	}

	template <typename FwdIt>
	void assign_dispatcher(FwdIt first, FwdIt last, std::forward_iterator_tag)
	{
		return assign_fwd_iterator(first, last);
	}

	template <typename BDIt>
	void assign_dispatcher(BDIt first, BDIt last, std::bidirectional_iterator_tag)
	{
		return assign_fwd_iterator(first, last);
	}

	template <typename RAIt>
	void assign_dispatcher(RAIt first, RAIt last, std::random_access_iterator_tag)
	{
		return assign_fwd_iterator(first, last);
	}

	template <typename FwdIt>
	void assign_fwd_iterator(FwdIt first, FwdIt last)
	{
		difference_type n = ft::distance(first, last);
		if (n < 0)
			throw std::length_error("vector");
		clear();
		if (static_cast<size_type>(n) > _capacity)
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

	template <typename InputIt>
	void insert_dispatcher(iterator position, InputIt first, InputIt last,
							std::input_iterator_tag)
	{
		while (first != last)
		{
			position = insert(position, *first++); // position is updated in case of reallocation
			position++;
		}
	}


	template <typename FwdIt>
	void insert_dispatcher(iterator position, FwdIt first, FwdIt last,
							std::forward_iterator_tag)
	{
		input_fwd_iterator(position, first, last);
	}

	template <typename BDIt>
	void insert_dispatcher(iterator position, BDIt first, BDIt last,
							std::bidirectional_iterator_tag)
	{
		input_fwd_iterator(position, first, last);
	}

	template <typename RAIt>
	void insert_dispatcher(iterator position, RAIt first, RAIt last,
							std::random_access_iterator_tag)
	{
		insert_fwd_iterator(position, first, last);
	}

	template <typename FwdIt>
	void insert_fwd_iterator(iterator position, FwdIt first, FwdIt last)
	{
		difference_type n;
		pointer insert_pos;

		n = ft::distance(first, last);
		if (n <= 0)
			return ;
		insert_pos = make_room4insertion(position, n);
		while (n--)
			_allocator.construct(insert_pos++, *first++);
	}

public:

	/*----------- Constructors, destructor and operator= ------------*/

	// Default constructor
	explicit vector(): _array(NULL), _size(0), _capacity(0)
	{
	} 

	// Fill constructor
	explicit vector(size_type n, value_type const & val = value_type()):
	_size(n), _capacity(n)
	{
		_array = _allocator.allocate(_capacity);
		for (size_t i = 0; i < _size; i++)
			_allocator.construct(_array + i, val);
	}

	// Range constructor
	template<typename I>
	vector(typename ft::enable_if<!ft::is_integral<I>::value, I>::type first,
			I last): _array(NULL), _size(0), _capacity(0)
	{
		assign(first, last);
	}

	// Copy constructor
	vector(vector const & x):
	_array(NULL), _size(0), _capacity(0)
	{
		*this = x;
	}

	// Destructor
	~vector()
	{
		if (_capacity)
		{
			for (size_type i = 0; i < _size; i++)
				_allocator.destroy(&_array[i]);
			_allocator.deallocate(_array, _capacity);
		}
	}

	// Assignation operator
	vector & operator=(vector const & x)
	{
		assign(x.begin(), x.end());
		return (*this);
	}

	/* --------------------- Iterators ----------------------- */

	iterator begin(void)
	{
		return (iterator(_array));
	}

	const_iterator begin(void) const
	{
		return (const_iterator(_array));
	}

	iterator end(void)
	{
		return (iterator(_array + _size));
	}

	const_iterator end(void) const
	{
		return (const_iterator(_array + _size));
	}

	reverse_iterator rbegin(void)
	{
		return (reverse_iterator(end()));
	}

	const_reverse_iterator rbegin(void) const
	{
		return (const_reverse_iterator(end()));
	}

	reverse_iterator rend(void)
	{
		return (reverse_iterator(begin()));
	}

	const_reverse_iterator rend(void) const
	{
		return (const_reverse_iterator(begin()));
	}

	/* -------------- Capacity: ------------------ */

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
				throw std::length_error("vector");
			reallocate(n);
		}
	}

	/* -------------- Element access ---------------- */

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
			throw (std::out_of_range("vector"));
		return (_array[n]);
	}

	const_reference at(size_type n) const
	{
		if (n >= _size)
			throw (std::out_of_range("vector"));
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
	
	/* ---------------- Modifiers ------------------ */

	// cppreference: The behavior of assign is undefined if either argument
	// is an iterator into *this.

	template < typename It >
	typename ft::enable_if<!is_integral<It>::value>::type	// return type: void
	assign(It first, It last)
	{
		assign_dispatcher(first, last,
			typename ft::iterator_traits<It>::iterator_category());
	}

	void assign(size_type n, value_type const & val)
	{
		clear();
		
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

	iterator insert(iterator position, value_type const & val)
	{
		pointer insert_position;

		insert_position = make_room4insertion(position, 1);
		_allocator.construct(insert_position, val);
		return (insert_position);
	}

	void insert(iterator position, size_type n, value_type const & val)
	{
		pointer insert_position;
		
		insert_position = make_room4insertion(position, n);
		while (n--)
			_allocator.construct(insert_position++, val);
	}

	template <typename I>
	typename ft::enable_if<!ft::is_integral<I>::value>::type  // return type: void
	insert(iterator position, I first, I last)
	{
		return (insert_dispatcher(position, first, last,
				typename ft::iterator_traits<I>::iterator_category()));
	}

	iterator erase(iterator position)
	{
		return (erase(position, position + 1));
	}

	iterator erase(iterator first, iterator last)
	{
		pointer ptr = _array + (first - begin());
		iterator it = first;
		iterator ite = end();

		while (it != last)
		{
			_allocator.destroy(&(*it++));
			_size -= 1;
		}
		while (it != ite)
		{
			_allocator.construct(ptr, *it);
			_allocator.destroy(&(*it));
			++ptr;
			++it;
		}
		return (first);
	}

	void swap (vector& x)
	{
		pointer		tmp_array = _array;
		size_type	tmp_size = _size;
		size_type	tmp_capacity = _capacity;

		_array = x._array;
		x._array = tmp_array;

		_size = x._size;
		x._size = tmp_size;

		_capacity = x._capacity;
		x._capacity = tmp_capacity;
	}

	void clear(void)
	{
		for (size_type i = 0; i < _size; i++)
			_allocator.destroy(&_array[i]);
		_size = 0;
	}

	/* ----------------- Allocator----------------------- */

	allocator_type get_allocator(void) const
	{
		return (_allocator);
	}
};


/* ----------------- Relational operators --------------------- */

template <typename T>
bool operator==(vector<T> const & lhs, vector<T> const & rhs)
{
	if (lhs.size() != rhs.size())
		return (false);
	return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename T>
bool operator!=(vector<T> const & lhs, vector<T> const & rhs)
{
	return (!(lhs == rhs));
}

template <typename T>
bool operator<(vector<T> const & lhs, vector<T> const & rhs)
{
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end()));
}

template <typename T>
bool operator>(vector<T> const & lhs, vector<T> const & rhs)
{
	return (rhs < lhs);
}

template <typename T>
bool operator<=(vector<T> const & lhs, vector<T> const & rhs)
{
	return !(rhs < lhs);
}

template <typename T>
bool operator>=(vector<T> const & lhs, vector<T> const & rhs)
{
	return !(lhs < rhs);
}

/* ----------------- Non-member functions overloads ----------------- */

template<typename T>
void swap(vector<T> & x, vector<T> & y)
{
	x.swap(y);
}


} // namespace ft


#endif