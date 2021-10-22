#ifndef FT_STACK_HPP
# define FT_STACK_HPP

# include "ft_vector.hpp"

namespace ft
{

template <typename T>
class stack
{
public:
	typedef T		 							value_type;
	typedef ft::vector<T>						container_type;
	typedef	typename container_type::size_type	size_type;

protected:
	container_type _container;

public:
	explicit stack (const container_type& ctnr = container_type()):
	_container(ctnr)
	{
	}

	bool empty() const
	{
		return (_container.empty());
	}

	size_type size() const
	{
		return (_container.size());

	}

	value_type& top()
	{
		return (_container.back());
	}

	void push(const value_type& val)
	{
		_container.push_back(val);
	}

	void pop()
	{
		_container.pop_back();
	}

	friend bool operator==(stack const & lhs, stack const & rhs)
	{
		return (lhs._container == rhs._container);
	}

	friend bool operator!=(stack const & lhs, stack const & rhs)
	{
		return (lhs._container != rhs._container);
	}

	friend bool operator<(stack const & lhs, stack const & rhs)
	{
		return (lhs._container < rhs._container);
	}

	friend bool operator>(stack const & lhs, stack const & rhs)
	{
		return (lhs._container > rhs._container);
	}

	friend bool operator<=(stack const & lhs, stack const & rhs)
	{
		return (lhs._container <= rhs._container);
	}

	friend bool operator>=(stack const & lhs, stack const & rhs)
	{
		return (lhs._container >= rhs._container);
	}
};


} // namespace ft


#endif