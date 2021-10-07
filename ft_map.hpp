#ifndef FT_MAP
# define FT_MAP

#include <iostream> // for debug messages

#include "utils.hpp"
#include "map_iterator.hpp"

namespace ft
{
	template <	typename Key,
				typename T > class map;
}

template < typename Key, typename T >
class ft::map
{
public:

	typedef	Key								key_type;
	typedef	T								mapped_type;
	typedef pair<const key_type, mapped_type> value_type;
	typedef std::size_t						size_type;
	typedef ft::map_iterator<value_type>	iterator;

private:

	typedef Node<value_type>	node;

	size_type	_size;
	node *		_root;
	node *		_end;

	node * get_node_address(iterator const & it)
	{
		// nasty stuff!!!
		return (*reinterpret_cast<node * const *>(&it));
	}

	ft::pair<iterator, bool> insert (node ** root, node *parent, value_type const & val)
	{
		node * current_node = *root;
		ft::pair<iterator, bool> ret;

		if (current_node == NULL)
		{
			*root = new node(val); // use std::allocator instead
			(*root)->parent = parent;
			_size += 1;
			ret.first = iterator(*root);
			ret.second = true;
			return (ret);
		}
		if (val.first < current_node->kv_pair.first) // change < for a comp function
			return (insert(&current_node->left, current_node, val));
		if (current_node->kv_pair.first < val.first)
			return (insert(&current_node->right, current_node, val));
		ret.first = iterator(current_node);
		ret.second = false;
		return (ret);
	}

	void clear(node **root)
	{
		if (*root)
		{
			clear(&(*root)->left);
			clear(&(*root)->right);
			// std::cout << "deleting " << (*root)->kv_pair.first << std::endl;
			delete (*root);	// use std::allocator instead
			*root = NULL;
		}
	}

	// returns pointer to node matching key k or _end if no match
	node * find(key_type const & k, node * root) const
	{
		if (!root)
			return (_end);
		if (k < root->kv_pair.first) // change for comp object
			return (find(k, root->left));
		if (k > root->kv_pair.first) // idem
			return (find(k, root->right));
		return (root);
	}

	// returns address of parent's left or right accordingly
	// returns left pointer in case _end is the parent
	node **parent_ptr_to_child(node *child) const
	{
		if (child->parent->left == child)
			return (&child->parent->left);
		return (&child->parent->right);
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
		delete target;
		_size -= 1;
		if (target == _root)
			_root = _end->left;
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

public:
	// default constructor
	map(): _size(0), _root(NULL)
	{
		_end = new node();
	}
	~map()
	{
		clear();
		delete _end;
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

	iterator insert (iterator position, value_type const & val)
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

	void clear()
	{
		clear(&_root);
		_size = 0;
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



};


#endif