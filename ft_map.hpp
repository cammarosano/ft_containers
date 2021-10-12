#ifndef FT_MAP
# define FT_MAP

#include <iostream> // for debug messages

#include "utils.hpp"
#include "map_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template <typename Key, typename T, typename Compare>
	class map;
}

template < typename Key, typename T, typename Compare = ft::less<Key> >
class ft::map
{
public:

	typedef	Key									key_type;
	typedef	T									mapped_type;
	typedef pair<const key_type, mapped_type>	value_type;
	typedef std::size_t							size_type;
	typedef ft::map_iterator<value_type>		iterator;
	typedef ft::reverse_iterator<iterator>		reverse_iterator;
	typedef Compare								key_compare;
	typedef std::allocator<value_type>			allocator_type;

private:

	typedef Node<value_type>	node;

	size_type		_size;
	node *			_root;
	node *			_end;
	key_compare		_compare;
	allocator_type	_allocator;
	std::allocator<node> _node_allocator;

	node * get_node_address(iterator const & it)
	{
		// nasty stuff!!!
		return (*reinterpret_cast<node * const *>(&it));
	}

	key_type const & key(node *n) const
	{
		return (n->content->first);
	}

	node *new_node(value_type const & val)
	{
		value_type	*pair;
		node		*n;

		pair = _allocator.allocate(1);
		_allocator.construct(pair, val);
		n = _node_allocator.allocate(1);
		_node_allocator.construct(n, node(pair));
		return (n);
	}

	void delete_node(node * &ptr)
	{
		_allocator.destroy(ptr->content);
		_allocator.deallocate(ptr->content, 1);
		_node_allocator.destroy(ptr);
		_node_allocator.deallocate(ptr, 1);
		ptr = NULL;
	}

	ft::pair<iterator, bool>
	insert (node ** root, node *parent, value_type const & val)
	{
		if (*root == NULL)
		{
			*root = new_node(val);
			(*root)->parent = parent;
			_size += 1;
			return (ft::make_pair(iterator(*root), true));
		}
		if (_compare(val.first, key(*root)))
			return (insert(&(*root)->left, *root, val));
		if (_compare(key(*root), val.first))
			return (insert(&(*root)->right, *root, val));
		return (ft::make_pair(iterator(*root), false));
	}

	void clear(node **root)
	{
		if (*root)
		{
			clear(&(*root)->left);
			clear(&(*root)->right);
			delete_node(*root);
		}
	}

	// returns pointer to node matching key k or _end if no match
	node * find(key_type const & k, node * root) const
	{
		if (!root)
			return (_end);
		if (_compare(k, key(root)))
			return (find(k, root->left));
		if (_compare(key(root), k))
			return (find(k, root->right));
		return (root);
	}

	// new_child could be NULL
	void update_parent_ptr2child(node *old_child, node *new_child)
	{
		node *parent = old_child->parent;
		if (!parent)
			return ;
		if (parent->left == old_child)
			parent->left = new_child;
		// these conditions are not mutually exclusive in case _end is the parent
		if (parent->right == old_child) 
			parent->right = new_child;
	}

	// find predecessor in subtree
	// root->left must NOT be NULL
	node *find_predecessor(node *root) const
	{
		root = root->left;
		while (root->right)
			root = root->right;
		return (root);
	}

	// no memory freeing
	void remove_node(node * target)
	{
		// 1. if node has no child, update parent's pointer
		if (!target->left && !target->right)
			update_parent_ptr2child(target, NULL);

		// 2. if node has one child, child replaces it
		// left case
		else if (target->left && !target->right)
		{
			update_parent_ptr2child(target, target->left);
			target->left->parent = target->parent;
		}
		// right case
		else if (target->right && !target->left)
		{
			update_parent_ptr2child(target, target->right);
			target->right->parent = target->parent;
		}
		else // 3. node has two children
		{
			node *predecessor = find_predecessor(target);
			remove_node(predecessor); 	// predec->right is NULL
			// replace target
			predecessor->left = target->left;
			if (target->left) // it could have been changed when predecessor was removed
				target->left->parent = predecessor;
			predecessor->right = target->right;
			target->right->parent = predecessor;
			predecessor->parent = target->parent;
			update_parent_ptr2child(target, predecessor);
		}
	}

	void erase_node(node *target)
	{
		remove_node(target);
		if (target == _root)
			_root = _end->left;
		// delete target;
		delete_node(target);
		_size -= 1;
	}

	bool check_hint(iterator position, key_type const & key)
	{
		if (position == end())
			return false;
		if (!_compare(position->first, key))
			return false;
		++position;
		if (position != end() && !_compare(key, position->first))
			return false;
		return true;
	}

	node * lower_bound(node *root, key_type const & k)
	{
		if (!root)
			return (NULL);
		if (_compare(key(root), k))
			return (lower_bound(root->right, k));
		if (_compare(k, key(root)))
		{
			node * better_candidate = lower_bound(root->left, k);
			if (better_candidate)
				return (better_candidate);
		}
		return (root);
	}

	node * upper_bound(node *root, key_type const & k)
	{
		if (!root)
			return (NULL);
		if (!_compare(k, key(root)))
			return (upper_bound(root->right, k));
		node * better_candidate = upper_bound(root->left, k);
		if (better_candidate)
			return (better_candidate);
		return (root);
	}

public:
	// default constructor
	map(): _size(0), _root(NULL)
	{
		_end = _node_allocator.allocate(1);
		_node_allocator.construct(_end, node(_allocator.allocate(1))); // pair is allocated but not constructed
	}
	~map() // consider writing helper functions for allocation and deallocation of _end
	{
		clear();
		_allocator.deallocate(_end->content, 1);
		_node_allocator.destroy(_end);
		_node_allocator.deallocate(_end, 1);
	}

	/* Iterators */

	iterator begin()
	{
		node *ptr = _end;
		while (ptr->left)
			ptr = ptr->left;
		return (iterator(ptr));
	}

	reverse_iterator rbegin()
	{
		return (reverse_iterator(end()));
	}

	iterator end()
	{
		return (iterator(_end));
	}

	reverse_iterator rend()
	{
		return (reverse_iterator(begin()));
	}
	
	/* Capacity */

	bool empty() const
	{
		return (!_size);
	}

	size_type size() const
	{
		return (_size);
	}

	size_type max_size() const
	{
		return (_allocator.max_size() * sizeof(value_type) /
						(sizeof(value_type) + sizeof(node)));
	}

	/* Element access */

	mapped_type & operator[](key_type const & k)
	{
		value_type pr(k, mapped_type());
		pair<iterator, bool> ret(insert(pr));
		return ((ret.first)->second);
	}

	/* Modifiers */

	ft::pair<iterator, bool> insert (value_type const & val)
	{
		bool	root_change;
		ft::pair<iterator, bool> ret;
	
		root_change = !(_root);
		ret = insert(&_root, _end, val);
		if (root_change)
		{
			_end->left = _root;
			_end->right = _root;
		}
		return (ret);
	}

	iterator insert(iterator position, value_type const & val)
	{
		node *predecessor;
		ft::pair<iterator, bool> ret;

		if (check_hint(position, val.first))
		{
			predecessor = get_node_address(position);
			ret = insert(&predecessor->right, predecessor, val);
		}
		else
			ret = insert(val);
		return (ret.first);
	}

	template <typename I>
	void insert(I first, I last)
	{
		iterator hint = end();
		while (first != last)
		{
			hint = insert(hint, *first);
			++first;
		}
	}
	
	size_type erase(key_type const & k)
	{
		node * target;
		
		target = find(k, _root);
		if (target == _end)
			return (0);
		erase_node(target);
		return (1);
	}

	void erase(iterator position)
	{
		node *target;
		
		target = get_node_address(position);
		if (target != _end) // must be a valid iterator though
			erase_node(target);
	}
	
	void erase(iterator first, iterator last)
	{
		node *target;

		while (first != last)
		{
			target = get_node_address(first);
			++first;
			erase_node(target);
		}
	}

	void swap(map& x)
	{
		node *temp_ptr;
		size_type temp_size;

		// swap _end
		temp_ptr = _end;
		_end = x._end;
		x._end = temp_ptr;

		// update _root
		_root = _end->left;
		x._root = x._end->left;

		// swap _size
		temp_size = _size;
		_size = x._size;
		x._size = temp_size;
	}

	void clear()
	{
		clear(&_root);
		_size = 0;
	}

	/* Operations */ 

	iterator find(key_type const & k)
	{
		return (iterator(find(k, _root)));
	}

	size_type count(key_type const & k) const
	{
		if (find(k, _root) == _end)
			return (0);
		return (1);
	}

	iterator lower_bound(key_type const & k)
	{
		node *ptr;

		ptr = lower_bound(_root, k);
		if (!ptr)
			return (end());
		return (iterator(ptr));
	}
	
	iterator upper_bound(key_type const & k)
	{
		node *ptr;

		ptr = upper_bound(_root, k);
		if (!ptr)
			return (end());
		return (iterator(ptr));
	}

	ft::pair<iterator, iterator> equal_range (key_type const & k)
	{
		return (ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
	}

	/* Allocator */
	allocator_type get_allocator() const
	{
		return (_allocator);
	}
};


#endif