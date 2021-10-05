#ifndef FT_MAP
# define FT_MAP

#include <iostream> // for debug messages

namespace ft
{
	template <	typename Key,
				typename T > class map;

	template < typename T1, typename T2 > struct pair
	{
		T1 first;
		T2 second;

		pair():  first(T1()), second(T2()) // is this value-initialization ?
		{
			// std::cout << "pair default constructor\n";
		}

		pair(T1 const & a, T2 const & b): first(a), second(b)
		{
			// std::cout << "pair initialization constructor\n";
		}

		// pair(pair const & src): first(src.first), second(src.second)
		// {
		// 	// std::cout << "pair copy construtor\n";
		// }

		template<typename U, typename V> pair(pair<U,V> const & pr):
		first(pr.first), second(pr.second)
		{
		}

	// raises deprecated error. It compiles with std=c++98
		// pair & operator=(pair const & rhs)
		// {
		// 	first = rhs.first;
		// 	second = rhs.second;
		// 	return (*this);
		// }

		// enable convertion from pair<T1, T2> to pair<const T1, T2>
		// operator pair<const T1, T2>() const
		// {
		// 	return (pair<const T1, T2>(first, second));
		// }
	};

	template <typename ValueType>
	struct Node
	{
		Node *		left;
		Node *		right;
		Node *		parent;
		ValueType	kv_pair;

		Node(ValueType const & item):
		left(NULL), right(NULL), parent(NULL), kv_pair(item)
		{
		}
		// default contructor
		Node():
		left(NULL), right(NULL), parent(NULL)
		{
		}
		~Node()
		{
		}

	};
	///// ITERATOR //////

	template <typename T>
	class map_iterator
	{
	public: // should I just inherit from a base class of iterator?

		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef T *								pointer;
		typedef T &								reference;
		typedef std::bidirectional_iterator_tag	iterator_category;

	private:
		typedef Node<value_type> node;

		node *_ptr;

		// root must not be NULL
		// returns the mininum node of a subtree
		node * min(node * root) const
		{
			while (root->left)
				root = root->left;
			return (root);
		}

		node * max(node * root) const
		{
			while (root->right)
				root = root->right;
			return (root);
		}

		// returns pointer to next node of sorted sequence
		node * next(node *ptr) const
		{
			if (ptr->right)
				return (min(ptr->right));
			while (ptr->parent)
			{
				std::cout << "ptr points to " << ptr->kv_pair.first << std::endl;
				if (ptr->parent->left == ptr) // ptr is a left child
					return (ptr->parent);
				ptr = ptr->parent;
			}
			return (ptr); // this line should not be reached
		}

		// returns pointer to previous node of sorted sequence
		node * previous(node *ptr) const
		{
			if (ptr->left)
				return (max(ptr->left));
			while (ptr->parent)
			{
				if (ptr->parent->right == ptr) // ptr is a right child
					return (ptr->parent);
				ptr = ptr->parent;
			}
			return (ptr);
		}

	public:
		map_iterator() {}
		map_iterator(node * address): _ptr(address) {}
		map_iterator(map_iterator const & src): _ptr(src._ptr) {}
		map_iterator & operator=(map_iterator const & src)
		{
			_ptr = src._ptr;
			return (*this);
		}

		reference operator*() const
		{
			return (_ptr->kv_pair);
		}

		pointer operator->() const
		{
			return (&_ptr->kv_pair);
		}

		map_iterator & operator++()
		{
			_ptr = next(_ptr);
			return (*this);
		}

		map_iterator operator++(int)
		{
			map_iterator temp(*this);

			_ptr = next(_ptr);
			return (temp);
		}

		map_iterator & operator--()
		{
			_ptr = previous(_ptr);
			return (*this);
		}

		map_iterator operator--(int)
		{
			map_iterator temp(*this);

			_ptr = previous(_ptr);
			return (temp);
		}

		bool operator==(map_iterator const & rhs)
		{
			return (_ptr == rhs._ptr);
		}

		bool operator!=(map_iterator const & rhs)
		{
			return (_ptr != rhs._ptr);
		}
	};


}

///// ft::map //////

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

	void update_end()
	{
		_end->left = _root;
		_end->right = _root;
		if (_root)
			_root->parent = _end;
	}

	// returns address of parent's left or right 
	node **parent_ptr_to_child(node *child) const
	{
		if (child->parent->left == child)
			return (&child->parent->left);
		return (&child->parent->right);
	}

	node *max(node *root) const
	{
		while (root->right)
			root = root->right;
		return (root);
	}

	// root->left must NOT be NULL
	node *find_predecessor(node *root) const
	{
		return (max(root->left));
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
		ft::pair<iterator, bool> ret = insert(&_root, NULL, val);
		update_end();
		return (ret);
	}

	void clear()
	{
		clear(&_root);
		_size = 0;
	}

	size_type erase(key_type const & k)
	{
		// find node
		node * target = find(k, _root);
		if (target == _end) // key not found
			return (0);

		// 1. if node has no child, delete node, update parent's pointer
		if (!target->left && !target->right)
		{
			*(parent_ptr_to_child(target)) = NULL;
			delete target;
			_size -= 1;
			if (target == _root)
			{
				_root = NULL;
				update_end();
			}
			return (1);
		}

		// 2. if node has one child, child replaces it (rearrange pointers)
		// left case
		if (target->left && !target->right)
		{
			*(parent_ptr_to_child(target)) = target->left;
			target->left->parent = target->parent;
			if (target == _root)
			{
				_root = target->left;
				update_end();
			}
			delete target;
			_size -= 1;
			return (1);
		}
		// right case
		if (target->right && !target->left)
		{
			*(parent_ptr_to_child(target)) = target->right;
			target->right->parent = target->parent;
			if (target == _root)
			{
				_root = target->right;
				update_end();
			}
			delete target;
			_size -= 1;
			return (1);
		}

		// 3. if node has two children,
		
		// 	find the predecessor (max in left sub-tree)
		node *predecessor = find_predecessor(target); // predecessor->right is NULL.
		// connect predecessor's parent with its left child
		if (predecessor->left)
			predecessor->left->parent = predecessor->parent;
		*(parent_ptr_to_child(predecessor)) = predecessor->left;
		// substitute target by predecessor
		predecessor->left = target->left;
		predecessor->right = target->right;
		predecessor->parent = target->parent;
		*(parent_ptr_to_child(target)) = predecessor;
		if (target == _root)
		{
			_root = predecessor;
			update_end();
			std::cout << _end->parent << std::endl;
			std::cout << _end->left << std::endl;
			std::cout << _end->right << std::endl;
			std::cout << _root << std::endl;
			std::cout << _root->parent << std::endl;
			std::cout << _end << std::endl;

		}
		delete target;
		_size -= 1;
		return (1);
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