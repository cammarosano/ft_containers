#ifndef FT_MAP
# define FT_MAP

#include <string> // debug

#include <iostream> // for debug messages

#include "Rbtree.hpp"
#include "utils.hpp"
#include "map_iterator.hpp"
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
	typedef std::size_t								size_type;
	typedef ft::map_iterator<value_type>			iterator;
	typedef ft::map_iterator<const value_type>		const_iterator;
	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef Compare									key_compare;
	typedef std::allocator<value_type>				allocator_type;

	class value_compare
	{
	private:
		key_compare _compare;
	
	public:
		value_compare(key_compare c): _compare(c) {} // made it public to avoid the use of friend

		bool operator() (value_type const & x, value_type const & y)
		{
			return (_compare(x.first, y.first));
		}

		// some typedefs here (see reference)
	};

private:

	typedef	::Node<value_type>	node;

	key_compare _compare;
	Rbtree<value_type, value_compare> _tree;

public:
	map();
	~map();
	

	ft::pair<iterator, bool> insert (value_type const & val);
	// to be continued!


	// debug utils
	void print_tree() const;

};



// Constructors

template <typename K, typename T, typename C>
map<K,T,C>::map(): _tree(value_compare(_compare))
{
}

template <typename K, typename T, typename C>
map<K,T,C>::~map()
{
}


template <typename K, typename T, typename C>
ft::pair<typename map<K,T,C>::iterator, bool>
map<K,T,C>::insert(value_type const &val)
{
	size_type	pre_size = _tree.size();
	iterator p = _tree.insert(val);
	return (ft::make_pair(p, (_tree.size() > pre_size)));	
}


// Debug utils

template <typename K, typename T, typename C>
void map<K,T,C>::print_tree() const
{
	_tree.print();
}

template <typename T1, typename T2>
std::ostream & operator<<(std::ostream & o, ft::pair<T1,T2> const & pair)
{
	o << "(" << pair.first << ", " << pair.second << ")";
	return (o);
}





} // namespace ft





#endif