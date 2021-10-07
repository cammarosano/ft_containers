#ifndef MAP_ITERATOR_HPP
# define MAP_ITERATOR_HPP

# include "utils.hpp"

namespace ft
{
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
		typedef ft::Node<value_type> node;

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
				// std::cout << "ptr points to " << ptr->kv_pair.first << std::endl;
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
#endif