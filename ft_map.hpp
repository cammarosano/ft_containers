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

		pair(): first(T1()), second(T2()) // is this value-initialization ?
		{
			// std::cout << "pair default constructor\n";
		}

		pair(T1 const & a, T2 const & b): first(a), second(b)
		{
			// std::cout << "pair initialization constructor\n";
		}

		pair(pair const & src): first(src.first), second(src.second)
		{
			// std::cout << "pair copy construtor\n";
		}

		pair & operator=(pair const & rhs)
		{
			first = rhs.first;
			second = rhs.second;
			return (*this);
		}

		// enable convertion from pair<T1, T2> to pair<const T1, T2>
		operator pair<const T1, T2>() const
		{
			return (pair<const T1, T2>(first, second));
		}
	};
	
}

template < typename Key, typename T >
class ft::map
{
public:
	typedef	Key								key_type;
	typedef	T								mapped_type;
	typedef pair<const key_type, mapped_type> value_type;
	typedef std::size_t						size_type;

private:
	struct Node
	{
		Node *		left;
		Node *		right;
		value_type	kv_pair;

		Node(value_type const & item):
		left(NULL), right(NULL), kv_pair(item)
		{
		}
		~Node()
		{
		}

	};
	
	size_type	_size;
	Node *		_root;

	bool insert (Node ** root, value_type const & val)
	{
		Node * current_node = *root;

		if (current_node == NULL)
		{
			*root = new Node(val); // use std::allocator instead
			_size += 1;
			return (true);
		}
		if (val.first < current_node->kv_pair.first) // change < for a comp function
			return (insert(&current_node->left, val));
		if (current_node->kv_pair.first < val.first)
			return (insert(&current_node->right, val));
		return (false);
	}

	void clear(Node **root)
	{
		if (*root)
		{
			clear(&(*root)->left);
			clear(&(*root)->right);
			std::cout << "deleting " << (*root)->kv_pair.first << std::endl;
			delete (*root);	// use std::allocator instead
			*root = NULL;
		}
	}


public:
	// default constructor
	map(): _size(0), _root(NULL)
	{
	}
	~map()
	{
		clear();
	}

	size_type size() const
	{
		return (_size);
	}

	bool empty() const
	{
		return (!_size);
	}

	// for now, return just a bool if value inserted
	bool insert (value_type const & val)
	{
		return (insert(&_root, val));
	}

	void clear()
	{
		clear(&_root);
		_size = 0;
	}

};

// TODO
/*
write an iterator
	iterator_category = bidirectional
	for now, just a pointer to a value_type (pair<key_type, value_type>)
	operator* returns a reference to the pair

change return type of insert to pair<iterator,bool>

write operator[] overload using the insert function

*/


#endif