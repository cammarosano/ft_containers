#ifndef FT_MAP
# define FT_MAP

#include <iostream> // for debug messages

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

	typedef Node<value_type>	node;

	size_type		_size;
	node *			_root;
	node *			_end;
	key_compare		_compare;
	allocator_type	_allocator;
	std::allocator<node> _node_allocator;


public: // for now!!!!
	node * get_node_address(iterator const & it)
	{
		// nasty stuff!!!
		return (*reinterpret_cast<node * const *>(&it));
	}

private:

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
		// _node_allocator.construct(n, node(pair));
		_node_allocator.construct(n, pair);
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
			node * inserted_node = new_node(val);
			*root = inserted_node;
			inserted_node->parent = parent;
			_size += 1;
			update_end();
			insert_rb_fix(inserted_node);
			return (ft::make_pair(iterator(inserted_node), true));
		}
		if (_compare(val.first, key(*root)))
			return (insert(&(*root)->left, *root, val));
		if (_compare(key(*root), val.first))
			return (insert(&(*root)->right, *root, val));
		return (ft::make_pair(iterator(*root), false));
	}

	void update_end(void)
	{
		_end->left = _root;
		_end->right = _root;
	}

	void clear(node * &root)
	{
		if (root)
		{
			clear(root->left);
			clear(root->right);
			delete_node(root);
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

	node * lower_bound(node *root, key_type const & k) const
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

	node * upper_bound(node *root, key_type const & k) const
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

	void setup_end(void)
	{
		_end = _node_allocator.allocate(1);
		// pair is allocated but not constructed
		// _node_allocator.construct(_end, node(_allocator.allocate(1)));
		_node_allocator.construct(_end, _allocator.allocate(1));
	}

	void clear_end(void)
	{
		_allocator.deallocate(_end->content, 1); // no pair to be destroyed
		_node_allocator.destroy(_end);
		_node_allocator.deallocate(_end, 1);
	}

	void copy_tree(node **root, node *parent, node *src)
	{
		if (!src)
		{
			*root = NULL;
			return ;
		}
		*root = new_node(*src->content);
		(*root)->parent = parent;
		copy_tree(&(*root)->left, *root, src->left);
		copy_tree(&(*root)->right, *root, src->right);
	}

public: // for now !!!!!!!!!

	// x->right cannot be NULL
	void left_rotate(node * x)
	{
		if (!x->right) // sanity check for debugging porpouses
		{
			std::cout << "left_rotate with x->right being NULL" << std::endl;
			return ;
		}

		node * y;

		y = x->right;
		// turn y's left subtree into x's right sub-tree
		x->right = y->left;
		if (y->left)
			y->left->parent = x;
		// turn x's parent into y's parent
		y->parent = x->parent;
		update_parent_ptr2child(x, y);
		// make x y's left child
		y->left = x;
		x->parent = y;

		// the root could have been changed
		_root = _end->left;
	}

// x->left cannot be NULL
void right_rotate(node *x)
{
	if (!x->left) // debug
	{
		std::cout << "right_rotate with x->left being NULL" << std::endl;
		return ;
	}

	node * y;

	y = x->left;
	// turn y's right subtree into x's left subtree
	x->left = y->right;
	if (y->right)
		y->right->parent = x;
	// turn x's parent into y's parent
	y->parent = x->parent;
	update_parent_ptr2child(x, y);
	// make x y's right child
	y->right = x;
	x->parent = y;

	// the root could have been changed
	_root = _end->left;
}

bool is_left_child(node *x)
{
	if (x->parent->left == x)
		return (true);
	return (false);
}

// allows to check the color of NULL nodes (always black)
bool is_red(node *x)
{
	if (x && x->color == node::red)
		return (true);
	return (false);
}

void insert_rb_fix(node * x)
{
	node * uncle;

	x->color = node::red;
	while (x != _root && x->parent->color == node::red)
	{
		if (is_left_child(x->parent))
		{
			uncle = x->parent->parent->right; 
			if (is_red(uncle)) // case 1: change colors
			{
				 x->parent->color = node::black;
				 uncle->color = node::black;
				 x->parent->parent->color = node::red;
				 // move x up the tree
				 x = x->parent->parent;
			}
			else	// y is black
			{
				if (!is_left_child(x)) // x is a right child
				{	// case 2 (triangle): move x up and rotate
					x = x->parent;
					left_rotate(x); // this leads always to case 3
				}
				// case 3 (line): recolor parent and grandparent, rotate right grandparent
				x->parent->color = node::black;
				x->parent->parent->color = node::red;
				right_rotate(x->parent->parent);
			}
		}
		else // x's parent is a right child
		{
			uncle = x->parent->parent->left;
			if (is_red(uncle))
			{
				x->parent->color = node::black;
				uncle->color = node::black;
				x->parent->parent->color = node::red;
				x = x->parent->parent;
			}
			else
			{
				if (is_left_child(x)) // triangle case
				{
					x = x->parent;
					right_rotate(x);
				}
				// line case
				x->parent->color = node::black;
				x->parent->parent->color = node::red;
				left_rotate(x->parent->parent);
			}
		}
	}
	// the root is always black
	_root->color = node::black;
}

// ------------------------------------------------------------------------
public:
	// default constructor
	explicit map(key_compare const & comp = key_compare()):
	_size(0), _root(NULL), _compare(comp)
	{
		setup_end();
	}

	// range constructor
	template <typename It>
	map(It first, It last, key_compare const & comp = key_compare()):
	_size(0), _root(NULL), _compare(comp)
	{
		setup_end();
		while (first != last)
		{
			insert(&_root, _end, *first);
			++first;
		}
		update_end();
	}

	// copy constructor
	map(map const & x):
	_root(NULL), _compare(x._compare) // needed in case Compare is a pointer to function
	{
		setup_end();
		*this = x;
	}

	~map()
	{
		clear();
		clear_end();
	}

	map & operator=(map const & x) // what to do with _compare ?
	{
		clear(_root);
		copy_tree(&_root, _end, x._root);
		update_end();
		_size = x._size;
		_compare = x._compare; // important in case it's a pointer to function
		return (*this);
	}


	/* Iterators */

	iterator begin()
	{
		node *ptr = _end;
		while (ptr->left)
			ptr = ptr->left;
		return (ptr);
	}

	const_iterator begin() const
	{
		node *ptr = _end;
		while (ptr->left)
			ptr = ptr->left;

		return (reinterpret_cast<Node<const value_type>*>(ptr));
	}

	iterator end()
	{
		return (_end);
	}

	const_iterator end() const
	{
		return (reinterpret_cast<Node<const value_type>*>(_end));
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
		// debug
		if (get_node_address(ret.first) == NULL)
			std::cout << "key: " << k << ", iterator points to NULL" << std::endl;
		//
		return ((ret.first)->second);
	}


	/* Modifiers */

	ft::pair<iterator, bool> insert (value_type const & val)
	{
		// bool	root_change;
		ft::pair<iterator, bool> ret;
	
		// root_change = !(_root);
		ret = insert(&_root, _end, val);
		// if (root_change)
		update_end();
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

	void erase(iterator position)
	{
		node *target;
		
		target = get_node_address(position);
		if (target != _end) // must be a valid iterator though
			erase_node(target);
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
		clear(_root);
		update_end();
		_size = 0;
	}


	/* Observers */ 

	key_compare key_comp(void) const
	{
		return (_compare);
	}

	value_compare value_comp(void) const
	{
		return (_compare);
	}


	/* Operations */ 

	iterator find(key_type const & k)
	{
		return (find(k, _root));
	}

	const_iterator find(key_type const & k) const
	{
		return (reinterpret_cast<Node<const value_type>*>(find(k, _root)));
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
	
	const_iterator lower_bound(key_type const & k) const
	{
		node *ptr;

		ptr = lower_bound(_root, k);
		if (!ptr)
			return (end());
		return (iterator(ptr)); // choose between this and reinterpret_cast
	}

	iterator upper_bound(key_type const & k)
	{
		node *ptr;

		ptr = upper_bound(_root, k);
		if (!ptr)
			return (end());
		return (iterator(ptr));
	}

	const_iterator upper_bound(key_type const & k) const
	{
		node *ptr;

		ptr = upper_bound(_root, k);
		if (!ptr)
			return (end());
		return (iterator(ptr));
	}

	ft::pair<iterator, iterator> equal_range(key_type const & k)
	{
		return (ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
	}

	ft::pair<const_iterator, const_iterator> equal_range(key_type const & k) const
	{
		return (ft::pair<const_iterator, const_iterator>(lower_bound(k),
														upper_bound(k)));
	}


	/* Allocator */

	allocator_type get_allocator() const
	{
		return (_allocator);
	}
};


/* Relational operators(map) */

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


/* swap overload */

template<typename Key, typename T, typename Compare>
void swap(map<Key, T, Compare> & x, map<Key, T, Compare> & y)
{
	x.swap(y);
}

} // ft namespace

#endif