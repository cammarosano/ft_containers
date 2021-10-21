#ifndef FT_SET_HPP
# define FT_SET_HPP

# include "Rbtree.hpp"
# include "tree_iterator.hpp"
# include "reverse_iterator.hpp"
# include "utils.hpp"

namespace ft
{


template < typename T, typename Compare = ft::less<T> >
class set
{
public:
	typedef T										key_type;
	typedef T										value_type;
	typedef Compare									key_compare;
	typedef Compare									value_compare;
	typedef std::allocator<value_type>				allocator_type;
	typedef tree_iterator<const value_type>			iterator;
	typedef tree_iterator<const value_type>			const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef	ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef std::size_t								size_type;

private:
	typedef RbNode<const value_type>	node;
	typedef Rbtree<const value_type, value_compare> tree_type;

	key_compare _key_comp;
	tree_type	_tree;

public:

	// ------ Constructors, Assignation Operator and Destructor -------//

	// Default constructor
	explicit set(const key_compare& comp = key_compare()):
	_key_comp(comp), _tree(_key_comp)
	{
	}

	// Copy constructor
	set(set const & x):
	_key_comp(x._key_comp), _tree(_key_comp)
	{
		*this = x;
	}

	// Range constructor
	template<typename It>
	set (It first, It last, key_compare const & comp = key_compare()):
	_key_comp(comp), _tree(_key_comp)
	{
		insert(first, last);
	}

	// Assignation operator overload
	set & operator=(set const & x)
	{
		_tree = x._tree;
		return (*this);
	}

	// Destructor
	~set() {}	

	// --------------- Iterators -------------------------- //

	iterator begin()
	{
		return (_tree.min());
	}

	const_iterator begin() const
	{
		return (_tree.min());

	}

	iterator end()
	{
		return(_tree.end());
	}

	const_iterator end() const
	{
		return(_tree.end());
	}

	reverse_iterator rbegin()
	{
		return (end());
	}

	const_reverse_iterator rbegin() const
	{
		return (end());
	}

	reverse_iterator rend()
	{
		return (begin());
	}

	const_reverse_iterator rend() const
	{
		return (begin());
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


	// ------------- Modifiers ----------------------------- //

	ft::pair<iterator,bool> insert (const value_type& val)	
	{
		size_type	pre_size = _tree.size();
		iterator	it = _tree.insert(val);
		return (ft::make_pair(it, (_tree.size() > pre_size)));	
	}

	iterator insert (iterator position, const value_type& val)
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

	void erase (iterator position)
	{
		_tree.erase(node_ptr(position));
	}

	size_type erase (const value_type& val)
	{
		return (_tree.erase(val));
	}

	void erase (iterator first, iterator last)
	{
		node *target;

		while (first != last)
		{
			target = node_ptr(first);
			++first;
			_tree.erase(target);
		}
	}

	void swap (set& x)
	{
		_tree.swap(x._tree);
	}

	void clear()
	{
		_tree.clear();
	}


	// ----------- Observers ----------------- //

	key_compare key_comp(void) const
	{
		return (_key_comp);
	}

	value_compare value_comp(void) const
	{
		return (_key_comp);
	}


	// ------------ Operations --------------- //

	iterator find (const value_type& val) const
	{
		return (_tree.find(val));
	}

	size_type count (const value_type& val) const
	{
		if (_tree.find(val) == _tree.end())
			return (0);
		return (1);
	}

	iterator lower_bound (const value_type& val) const
	{
		return (_tree.lower_bound(val));
	}

	iterator upper_bound (const value_type& val) const
	{
		return (_tree.upper_bound(val));
	}

	pair<iterator,iterator> equal_range (const value_type& val) const
	{
		return (ft::make_pair(lower_bound(val), upper_bound(val)));
	}


	// --------- Allocator ------------ //

	allocator_type get_allocator() const
	{
		return (_tree.get_allocator());
	}


	// Debug utils ==========================================

	void print_tree() const
	{
		_tree.print();
	}
	// =======================================================

private:
	node * node_ptr(iterator const & it)
	{
		return (*reinterpret_cast<node * const *>(&it));
	}

};

// -------- Relational operators (set) ------------- //

template <typename T, typename C>
bool operator==(const set<T,C>& lhs, const set<T,C>& rhs)
{
	if (lhs.size() != rhs.size())
		return (false);
	return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename T, typename C>
bool operator!=(const set<T,C>& lhs, const set<T,C>& rhs)
{
	return (!(lhs == rhs));
}

template <typename T, typename C>
bool operator<(const set<T,C>& lhs, const set<T,C>& rhs)
{
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end()));
}

template <typename T, typename C>
bool operator<=(const set<T,C>& lhs, const set<T,C>& rhs)
{
	return (!(rhs < lhs));
}

template <typename T, typename C>
bool operator>(const set<T,C>& lhs, const set<T,C>& rhs)
{
	return (rhs < lhs);
}
	
template <typename T, typename C>
bool operator>=(const set<T,C>& lhs, const set<T,C>& rhs)
{
	return (!(lhs < rhs));

}


// ------------- Swap overload ---------------- //

template <typename T, typename C>
void swap(set<T,C> & x, set<T,C> & y)
{
	x.swap(y);
}


} // namespace ft


#endif