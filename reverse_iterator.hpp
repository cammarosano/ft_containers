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
	typedef typename std::iterator_traits<I>::pointer_type			pointer_type;
	typedef typename std::iterator_traits<I>::reference				reference;

private:
	iterator_type _base;

public:
	reverse_iterator(void)  {} // not sure what to do with _base
	reverse_iterator(iterator_type it): _base(it) {}
	reverse_iterator(reverse_iterator const & src) // copy constructor
	{
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
		return (*this);
	}




};

#endif