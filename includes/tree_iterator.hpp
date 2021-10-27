#ifndef TREE_ITERATOR_HPP
# define TREE_ITERATOR_HPP

#include <iterator> // bidirection_iterator_tag
#include "Rbtree.hpp" // RbNode

namespace ft
{
	template <typename T>
	class tree_iterator
	{
	public:

		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef value_type *					pointer;
		typedef value_type &					reference;
		typedef std::bidirectional_iterator_tag	iterator_category;

	private:
		typedef RbNode<T> node;

		node const *_ptr;

		// root must not be NULL
		// returns the mininum node of a subtree
		node const * min(node const * root) const
		{
			while (root->left)
				root = root->left;
			return (root);
		}

		node const * max(node const * root) const
		{
			while (root->right)
				root = root->right;
			return (root);
		}

		// returns pointer to next node of sorted sequence
		node const * next(node const * ptr) const
		{
			if (ptr->right)
				return (min(ptr->right));
			while (ptr->parent)
			{
				if (ptr->parent->left == ptr) // ptr is a left child
					return (ptr->parent);
				ptr = ptr->parent;
			}
			return (ptr);
		}

		// returns pointer to previous node of sorted sequence
		node const * previous(node const *ptr) const
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
		tree_iterator() {}
		tree_iterator(node const * address): _ptr(address) {}
		tree_iterator(tree_iterator const & src): _ptr(src._ptr) {}
		~tree_iterator() {};
		tree_iterator & operator=(tree_iterator const & src)
		{
			_ptr = src._ptr;
			return (*this);
		}

		// iterator must be convertible to const_iterator
		operator tree_iterator<const T>() const
		{
			return (tree_iterator<const T>
				(reinterpret_cast<RbNode<const T> const *>(_ptr)));
		}

		reference operator*() const
		{
			return (*_ptr->value);
		}

		pointer operator->() const
		{
			return (_ptr->value);
		}

		tree_iterator & operator++()
		{
			_ptr = next(_ptr);
			return (*this);
		}

		tree_iterator operator++(int)
		{
			tree_iterator temp(*this);

			_ptr = next(_ptr);
			return (temp);
		}

		tree_iterator & operator--()
		{
			_ptr = previous(_ptr);
			return (*this);
		}

		tree_iterator operator--(int)
		{
			tree_iterator temp(*this);

			_ptr = previous(_ptr);
			return (temp);
		}

		bool operator==(tree_iterator const & rhs)
		{
			return (_ptr == rhs._ptr);
		}

		bool operator!=(tree_iterator const & rhs)
		{
			return (_ptr != rhs._ptr);
		}
	};

}
#endif