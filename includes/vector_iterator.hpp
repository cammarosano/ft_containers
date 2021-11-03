/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_iterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:39:22 by rcammaro          #+#    #+#             */
/*   Updated: 2021/11/03 14:54:30 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <iterator> // category tag

namespace ft
{

template <typename T>
class vector_iterator
{
public:
	typedef std::ptrdiff_t					difference_type;
	typedef T								value_type;
	typedef T *								pointer;
	typedef T &								reference;
	typedef std::random_access_iterator_tag	iterator_category;

private:
	T *	ptr;

public:
		vector_iterator() {}
		vector_iterator(value_type *address): ptr(address) {}
		vector_iterator(vector_iterator const & src): ptr(src.ptr) {}
		~vector_iterator() {}

		// enabling implict conversion from normal it to const_iterator (cast operator)
		operator vector_iterator<const T>() const
		{
			return (vector_iterator<const T>(ptr));
		}

		vector_iterator & operator=(vector_iterator const & rhs)
		{
			ptr = rhs.ptr;
			return (*this);
		}

		value_type & operator*() const
		{
			return (*ptr);
		}

		vector_iterator & operator++()	// prefix increment
		{
			ptr += 1;
			return (*this);
		}

		vector_iterator operator++(int) // postfix increment (returns a new object)
		{
			vector_iterator temp(*this);

			ptr += 1;
			return (temp);
		}

		vector_iterator & operator--()
		{
			ptr -= 1;
			return (*this);
		}

		vector_iterator operator--(int)
		{
			vector_iterator temp(*this);

			ptr -= 1;
			return (temp);
		}

		vector_iterator operator+(difference_type n) const 
		{
			return (vector_iterator(ptr + n));
		}

		vector_iterator operator-(difference_type n) const 
		{
			return (vector_iterator(ptr - n));
		}

		vector_iterator & operator+=(difference_type n)
		{
			ptr = ptr + n;
			return (*this);
		}	

		vector_iterator & operator-=(difference_type n)
		{
			ptr = ptr - n;
			return (*this);
		}	

		value_type & operator[](difference_type i) const
		{
			return (ptr[i]);
		}

		pointer	operator->() const
		{
			return (ptr);
		}

		/* Non-member functions overloads */

		friend difference_type 
		operator-(vector_iterator const & lhs, vector_iterator const & rhs)
		{
			return (lhs.ptr - rhs.ptr);
		}

		friend bool
		operator==(vector_iterator const & lhs, vector_iterator const & rhs) 
		{
			return (lhs.ptr == rhs.ptr);
		}

		friend bool
		operator!=(vector_iterator const & lhs, vector_iterator const & rhs)
		{
			return (lhs.ptr != rhs.ptr);
		}
		
		friend bool
		operator>(vector_iterator const & lhs, vector_iterator const & rhs) 
		{
			return (lhs.ptr > rhs.ptr);
		}

		friend bool
		operator<(vector_iterator const & lhs, vector_iterator const & rhs) 
		{
			return (lhs.ptr < rhs.ptr);
		}

		friend bool
		operator>=(vector_iterator const & lhs, vector_iterator const & rhs) 
		{
			return (lhs.ptr >= rhs.ptr);
		}

		friend bool
		operator<=(vector_iterator const & lhs, vector_iterator const & rhs) 
		{
			return (lhs.ptr <= rhs.ptr);
		}

};

template <typename T>
vector_iterator<T> operator+(typename vector_iterator<T>::difference_type n,
							vector_iterator<T> it)
{
	return (it + n);
}

} // namespace ft

#endif
