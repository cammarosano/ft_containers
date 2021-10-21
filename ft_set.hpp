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


	// TODO: other constructors

	// Destructor
	~set() {}	

	// --------------- Iterators -------------------------- //

	iterator begin()
	{
		return (_tree.min());
	}

	iterator end()
	{
		return(_tree.end());
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


	// Debug utils

	void print_tree() const
	{
		_tree.print();
	}

private:
	node * node_ptr(iterator const & it)
	{
		return (*reinterpret_cast<node * const *>(&it));
	}

};
	
} // namespace ft


#endif