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

		// pair & operator=(pair const & rhs)
		// {
			// first = rhs.first;
			// second = rhs.second;
			// return (*this);
		// }

		// enable convertion from pair<T1, T2> to pair<const T1, T2>
		operator pair<const T1, T2>() const
		{
			return (pair<const T1, T2>(first, second));
		}
	};

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
		value_type *ptr;

	public:
		map_iterator() {}
		map_iterator(pointer p): ptr(p) {}
		map_iterator(map_iterator const & src): ptr(src.ptr) {}
		map_iterator & operator=(map_iterator const & src)
		{
			ptr = src.ptr;
			return (*this);
		}

		reference operator*() const
		{
			return (*ptr);
		}

		pointer operator->() const
		{
			return (ptr);
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
	typedef ft::map_iterator<value_type>	iterator;

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

	ft::pair<iterator, bool> insert (Node ** root, value_type const & val)
	{
		Node * current_node = *root;
		ft::pair<iterator, bool> ret;

		if (current_node == NULL)
		{
			*root = new Node(val); // use std::allocator instead
			_size += 1;
			ret.first = &(*root)->kv_pair;
			ret.second = true;
			return (ret);
		}
		if (val.first < current_node->kv_pair.first) // change < for a comp function
			return (insert(&current_node->left, val));
		if (current_node->kv_pair.first < val.first)
			return (insert(&current_node->right, val));
		ret.first = &current_node->kv_pair;
		ret.second = false;
		return (ret);
	}

	void clear(Node **root)
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

	ft::pair<iterator, bool> insert (value_type const & val)
	{
		return (insert(&_root, val));
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
	

};


#endif