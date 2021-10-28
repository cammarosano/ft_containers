#ifndef FT_MAP
# define FT_MAP

#include <string> // debug
#include <iostream> // for debug messages

#include "Rbtree.hpp"
#include "utils.hpp"
#include "tree_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft
{

template < typename Key, typename T, typename Compare = ft::less<Key> >
class map
{
public:

	typedef	Key										key_type;
	typedef	T										mapped_type;
	typedef pair<const key_type, mapped_type>		value_type;
	typedef Compare									key_compare;
	typedef std::allocator<value_type>				allocator_type;
	typedef value_type&								reference;
	typedef value_type const &						const_reference;
	typedef value_type*								pointer;
	typedef value_type const *						const_pointer;
	typedef ft::tree_iterator<value_type>			iterator;
	typedef ft::tree_iterator<const value_type>		const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef std::ptrdiff_t							difference_type;
	typedef std::size_t								size_type;

	class value_compare
	{
	private:
		key_compare comp;
	
	public:
		value_compare(key_compare c): comp(c) {} 

		bool operator() (value_type const & x, value_type const & y) const
		{
			return (comp(x.first, y.first));
		}

		typedef bool		result_type;
		typedef value_type	first_argument_type;
		typedef value_type	second_argument_type;
	};

private:

	typedef	RbNode<value_type>	node;
	typedef Rbtree<value_type, value_compare> tree_type;

	key_compare	_key_comp;
	tree_type	_tree;

	node * node_ptr(iterator const & it)
	{
		return (*reinterpret_cast<node * const *>(&it));
	}

public:

	// ------ Constructors, Assignation Operator and Destructor -------//

	// Default constructor
	explicit map(key_compare const & comp = key_compare()):
	_key_comp(comp), _tree(value_compare(_key_comp))
	{
	}

	// Range constructor
	template <typename It>
	map(It first, It last, key_compare const & comp = key_compare()):
	_key_comp(comp), _tree(value_compare(_key_comp))
	{
		insert(first, last);
	}

	// Copy constructor
	map(map const & x):
	_key_comp(x._key_comp), _tree(value_compare(_key_comp))
	{
		*this = x;
	}

	// Assignation operator overload
	map & operator=(map const & x)
	{
		_tree = x._tree;
		return (*this);
	}

	// Destructor
	~map()
	{
	}
	
	// --------------- Iterators -------------------------- //

	iterator begin()
	{
		return (_tree.min());
	}

	const_iterator begin() const
	{
		return (iterator(_tree.min()));
	}

	iterator end()
	{
		return(_tree.end());
	}

	const_iterator end() const
	{
		return (iterator(_tree.end()));
	}

	reverse_iterator rbegin()
	{
		return (reverse_iterator(end()));
	}

	const_reverse_iterator rbegin() const
	{
		return (const_reverse_iterator(end()));
	}

	reverse_iterator rend()
	{
		return (reverse_iterator(begin()));
	}

	const_reverse_iterator rend() const
	{
		return (const_reverse_iterator(begin()));
	}

	// ------------- Capacity ------------------------ //

	bool empty() const
	{
		return (!size());
	}

	size_type size() const
	{
		return (_tree.size());
	}

	size_type max_size() const
	{
		return (_tree.max_size());
	}

	// ------------- Element access ------------------------ //

	mapped_type & operator[](key_type const & k)
	{
		value_type value(k, mapped_type());
		return ((insert(value).first)->second);
	}


	// ------------- Modifiers ----------------------------- //

	ft::pair<iterator, bool> insert (value_type const & val)
	{
		size_type	pre_size = _tree.size();
		iterator	it = _tree.insert(val);
		return (ft::make_pair(it, (_tree.size() > pre_size)));	
	}

	iterator insert(iterator position, value_type const & val)
	{
		iterator it = _tree.insert(node_ptr(position), val);
		return (it);
	}

	template <typename It>
	void insert(It first, It last)
	{
		iterator hint = end();
		while (first != last)
		{
			hint = insert(hint, *first);
			++first;
		}
	}

	void erase(iterator position)
	{
		_tree.erase(node_ptr(position));
	}

	size_type erase(key_type const & k)
	{
		value_type value(k, mapped_type());
		return (_tree.erase(value));
	}

	void erase(iterator first, iterator last)
	{
		node *target;

		while (first != last)
		{
			target = node_ptr(first);
			++first;
			_tree.erase(target);
		}
	}

	void swap(map& x)
	{
		_tree.swap(x._tree);
	}

	void clear()
	{
		_tree.clear();
	}

	// ----------- Observers -----------------//

	key_compare key_comp(void) const
	{
		return (_key_comp);
	}

	value_compare value_comp(void) const
	{
		return (value_compare(_key_comp));
	}

	// ----------- Operations --------------- //

	iterator find(key_type const & k)
	{
		value_type value(k, mapped_type());
		return (_tree.find(value));
	}

	const_iterator find(key_type const & k) const
	{
		value_type value(k, mapped_type());
		return (iterator(_tree.find(value)));
	}

	size_type count(key_type const & k) const
	{
		value_type value(k, mapped_type());

		if (_tree.find(value) == _tree.end())
			return (0);
		return (1);
	}

	iterator lower_bound(key_type const & k)
	{
		value_type value(k, mapped_type());

		return (_tree.lower_bound(value));
	}

	const_iterator lower_bound(key_type const & k) const
	{
		value_type value(k, mapped_type());

		return (iterator(_tree.lower_bound(value)));
	}

	iterator upper_bound(key_type const & k)
	{
		value_type value(k, mapped_type());

		return (_tree.upper_bound(value));
	}

	const_iterator upper_bound(key_type const & k) const
	{
		value_type value(k, mapped_type());

		return (iterator(_tree.upper_bound(value)));
	}

	ft::pair<iterator, iterator> equal_range(key_type const & k)
	{
		return (ft::make_pair(lower_bound(k), upper_bound(k)));
	}

	ft::pair<const_iterator, const_iterator>
	equal_range(key_type const & k) const
	{
		return (ft::make_pair(lower_bound(k), upper_bound(k)));
	}

	// ----------- Allocator --------------- //

	allocator_type get_allocator() const
	{
		return (_tree.get_allocator());
	}

};

// ---------- Relational Operators (map) ------------ //

template<typename Key, typename T, typename Compare>
bool operator==(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	if (!(lhs.size() == rhs.size()))
		return (false);
	return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template<typename Key, typename T, typename Compare>
bool operator!=(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	return (!(lhs == rhs));
}

template<typename Key, typename T, typename Compare>
bool operator<(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end()));
}

template<typename Key, typename T, typename Compare>
bool operator<=(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	return (!(rhs < lhs));
}

template<typename Key, typename T, typename Compare>
bool operator>(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	return (rhs < lhs);
}

template<typename Key, typename T, typename Compare>
bool operator>=(map<Key, T, Compare> const & lhs,
				map<Key, T, Compare> const & rhs)
{
	return (!(lhs < rhs));
}

//------------- Swap overload ------------------- //

template<typename Key, typename T, typename Compare>
void swap(map<Key, T, Compare> & x, map<Key, T, Compare> & y)
{
	x.swap(y);
}

} // namespace ft


#endif