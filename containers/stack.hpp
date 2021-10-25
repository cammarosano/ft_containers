#ifndef FT_STACK_HPP
# define FT_STACK_HPP

# include "vector.hpp"

namespace ft
{

template <typename T, typename Container = vector<T> >
class stack
{
public:
	typedef T		 							value_type;
	typedef Container							container_type;
	typedef	typename container_type::size_type	size_type;

protected:
	container_type c;

public:
	explicit stack (const container_type& ctnr = container_type()):
	c(ctnr)
	{
	}

	bool empty() const
	{
		return (c.empty());
	}

	size_type size() const
	{
		return (c.size());
	}

	value_type& top()
	{
		return (c.back());
	}

	void push(const value_type& val)
	{
		c.push_back(val);
	}

	void pop()
	{
		c.pop_back();
	}

	friend bool operator==(stack const & lhs, stack const & rhs)
	{
		return (lhs.c == rhs.c);
	}

	friend bool operator!=(stack const & lhs, stack const & rhs)
	{
		return (lhs.c != rhs.c);
	}

	friend bool operator<(stack const & lhs, stack const & rhs)
	{
		return (lhs.c < rhs.c);
	}

	friend bool operator>(stack const & lhs, stack const & rhs)
	{
		return (lhs.c > rhs.c);
	}

	friend bool operator<=(stack const & lhs, stack const & rhs)
	{
		return (lhs.c <= rhs.c);
	}

	friend bool operator>=(stack const & lhs, stack const & rhs)
	{
		return (lhs.c >= rhs.c);
	}
};


} // namespace ft


#endif