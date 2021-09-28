#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

#include "iterator.hpp"
#include <iterator>		// iterator_traits

namespace ft
{
	template <typename I> class reverse_iterator;
}

template <typename I>
class ft::reverse_iterator
{
public:
	// preserving iterator "I"'s traits
	typedef I														iterator_type;
	typedef typename std::iterator_traits<I>::iterator_category		iterator_category;
	typedef typename std::iterator_traits<I>::value_type			value_type;
	typedef typename std::iterator_traits<I>::difference_type		difference_type;
	typedef typename std::iterator_traits<I>::pointer				pointer;
	typedef typename std::iterator_traits<I>::reference				reference;

private:
	iterator_type _base;

public:
	reverse_iterator(void)  {} // not sure what to do with _base
	reverse_iterator(iterator_type it): _base(it) {}
	reverse_iterator(reverse_iterator const & src):	 // copy constructor (DIFFERENT FROM THE REFERENCE!!)
	_base(src._base) {}

	reverse_iterator & operator=(reverse_iterator const & rhs)
	{
		_base = rhs._base;
		return (*this);
	}

	iterator_type base(void) const
	{
		return (_base);
	}

	reference operator*() const
	{
		iterator_type base_copy(_base);

		return (*(--base_copy));
	}

	reverse_iterator & operator++() // pre-increment
	{
		--_base;
		return (*this);
	}

	reverse_iterator operator++(int) // post-increment
	{
		reverse_iterator temp(*this); // todo: copy constructor
		--_base;
		return (temp);
	}

	reverse_iterator & operator--() // pre-decrement
	{
		++_base;
		return (*this);
	}

	reverse_iterator operator--(int) // post-decrement
	{
		reverse_iterator temp(*this);
		++_base;
		return (temp);
	}

	reverse_iterator operator+(difference_type n) const
	{
		return (reverse_iterator(_base - n));
	}

	reverse_iterator operator-(difference_type n) const
	{
		return (reverse_iterator(_base + n));
	}

	reverse_iterator & operator+=(difference_type n)
	{
		_base -= n;
		return (*this);
	}

	reverse_iterator & operator-=(difference_type n)
	{
		_base += n;
		return (*this);
	}
};

/* relational operators overloads */

template <typename I>
bool operator==(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() == rhs.base());
}

template <typename I>
bool operator!=(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() != rhs.base());
}

template <typename I>
bool operator<(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() > rhs.base());
}

template <typename I>
bool operator<=(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() >= rhs.base());
}

template <typename I>
bool operator>(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() < rhs.base());
}

template <typename I>
bool operator>=(ft::reverse_iterator<I> const & lhs,
				ft::reverse_iterator<I> const & rhs)
{
	return (lhs.base() <= rhs.base());
}


/* + operator */

template <typename I>
ft::reverse_iterator<I>
operator+(typename ft::reverse_iterator<I>::difference_type n,
			ft::reverse_iterator<I> rit)
{
	return (rit + n);
}

/* - operator between 2 iterators */
template <typename I>
typename ft::reverse_iterator<I>::difference_type operator-
(ft::reverse_iterator<I> lhs, ft::reverse_iterator<I> rhs)
{
	return (rhs.base() - lhs.base());
}


#endif