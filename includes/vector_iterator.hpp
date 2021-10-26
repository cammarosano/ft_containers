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
		vector_iterator()
		{
			// std::cout << "iterator default constructor\n";
		}
		vector_iterator(value_type *address): ptr(address)
		{
			// std::cout << "iterator parameter constructor\n";
		}
		vector_iterator(vector_iterator const & src): ptr(src.ptr) // copy constructor
		{
		}
		virtual ~vector_iterator()
		{
		}
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
			ptr += 1; // pointer arithmetic must be supported by value_type *
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
		difference_type operator-(vector_iterator const & rhs) const
		{
			return (ptr - rhs.ptr);
		}
		bool operator==(vector_iterator const & rhs) const
		{
			return (ptr == rhs.ptr);
		}
		bool operator!=(vector_iterator const & rhs) const
		{
			return (ptr != rhs.ptr);
		}
		bool operator>(vector_iterator const & rhs) const
		{
			return (ptr > rhs.ptr);
		}
		bool operator<(vector_iterator const & rhs) const
		{
			return (ptr < rhs.ptr);
		}
		bool operator>=(vector_iterator const & rhs) const
		{
			return (ptr >= rhs.ptr);
		}
		bool operator<=(vector_iterator const & rhs) const
		{
			return (ptr <= rhs.ptr);
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
vector_iterator<T> operator+(typename vector_iterator<T>::difference_type n,
							vector_iterator<T> it)
{
	return (it + n);
}

} // namespace ft

#endif
