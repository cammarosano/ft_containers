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

	size_type size() const
	{
		return (_size);
	}

	bool empty() const
	{
		return (!_size);
	}

	ft::pair<iterator, bool> insert (value_type const & val)
	{
		ft::pair<iterator, bool> ret = insert(&_root, NULL, val);
		// update _end
		_end->left = _root;
		_end->right = _root;
		_root->parent = _end;
		return (ret);
	}

	void clear()
	{
		clear(&_root);
		_size = 0;
	}

	mapped_type & operator[](key_type const & k)
	{
		value_type pr(k, mapped_type());
		pair<iterator, bool> ret(insert(pr));
		return ((ret.first)->second);
	}

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
	

};


#endif