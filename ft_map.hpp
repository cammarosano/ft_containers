#ifndef FT_MAP
# define FT_MAP

#include <iostream> // for debug messages

#include "utils.hpp"
#include "map_iterator.hpp"

namespace ft
{
	template <typename T>
	struct less
	{
		bool operator()(T const & x, T const & y) const
		{
			return (x < y);
		}
	};

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
	typedef Compare								key_compare;
	typedef std::allocator<value_type>			allocator_type;

private:

	typedef Node<value_type>	node;
	typedef std::allocator<node>	node_allocator_type;

	size_type		_size;
	node *			_root;
	node *			_end;
	key_compare		_compare;
	node_allocator_type	_node_allocator;
	allocator_type	_allocator;	// the one I don't actually use

	node * get_node_address(iterator const & it)
	{
		// nasty stuff!!!
		return (*reinterpret_cast<node * const *>(&it));
	}

	void delete_node(node * &ptr)
	{
		_node_allocator.destroy(ptr);
		_node_allocator.deallocate(ptr, 1);
		ptr = NULL;
	}

	ft::pair<iterator, bool>
	insert (node ** root, node *parent, value_type const & val)
	{
		if ((*root) == NULL)
		{
			// *root = new node(val); // use std::allocator instead
			*root = _node_allocator.allocate(1);
			_node_allocator.construct(*root, node(val));
			(*root)->parent = parent;
			_size += 1;
			return (ft::make_pair(iterator(*root), true));
		}
		if (_compare(val.first, (*root)->kv_pair.first))
			return (insert(&(*root)->left, *root, val));
		if (_compare((*root)->kv_pair.first, val.first))
			return (insert(&(*root)->right, *root, val));
		return (ft::make_pair(iterator(*root), false));
	}

	void clear(node **root)
	{
		if (*root)
		{
			clear(&(*root)->left);
			clear(&(*root)->right);
			// delete (*root);	// use std::allocator instead
			delete_node(*root);
			// _allocator.destroy(*root);
			// _allocator.deallocate(*root, 1);
			// *root = NULL;
		}
	}

	// returns pointer to node matching key k or _end if no match
	node * find(key_type const & k, node * root) const
	{
		if (!root)
			return (_end);
		if (_compare(k, root->kv_pair.first))
			return (find(k, root->left));
		if (_compare(root->kv_pair.first, k))
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
		if (!(position->first < key))
			return false;
		++position;
		if (position != end() && !(key < (position->first)))
			return false;
		return true;
	}

	node * lower_bound(node *root, key_type const & k)
	{
		if (!root)
			return (NULL);
		if (root->kv_pair.first < k)
			return (lower_bound(root->right, k));
		if (k < root->kv_pair.first)
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
		if (!(k < root->kv_pair.first))
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
		// _end = new node();
		_end = _node_allocator.allocate(1);
		_node_allocator.construct(_end, node());
	}
	~map()
	{
		clear();
		// delete _end;
		delete_node(_end);
	}

	/* Iterators */

	iterator begin()
	{
		node *ptr = _end;
		while (ptr->left)
			ptr = ptr->left;
		return (iterator(ptr));
	}

	iterator end()
	{
		return (iterator(_end));
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
			// std::cout << "good hint for inserting " << val.first << std::endl;
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