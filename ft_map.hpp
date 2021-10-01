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
			std::cout << "pair default constructor\n";

		}

		pair(T1 const & a, T2 const & b): first(a), second(b)
		{
			std::cout << "pair initialization constructor\n";
		}

		pair(pair const & src): first(src.first), second(src.second)
		{
			std::cout << "pair copy construtor\n";
		}

		pair & operator=(pair const & rhs)
		{
			first = rhs.first;
			second = rhs.second;
			return (*this);
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
	struct Btree
	{
		Btree *		left;
		Btree *		right;
		value_type	data;

		Btree(value_type const & item): left(NULL), right(NULL), data(item)
		{
		}
		~Btree()
		{
			// delete child subtrees ?
		}

	};
	
	size_type	_size;
	Btree	 *	_root;

	bool insert_data (Btree ** root, value_type const & val)
	{
		if (!*root)
		{
			*root = new Btree(val);
			_size += 1;
			return (true);
		}
		if (val.first < (*root)->data.first) // change < for a comp function
			return (insert_data(&(*root)->left, val));
		if (root->data.first < val.first)
			return (insert_data(&(*root)->right, val));
		return (false);
	}

public:
	map(): _size(0), _root(NULL)
	{
	}
	~map()
	{
	}

	size_type size() const
	{
		return (_size);
	}

	// for now, return just a bool if value inserted
	bool insert (value_type const & val)
	{
		return (insert_data(&_root, val));
	}
};



#endif