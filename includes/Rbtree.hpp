/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:37:45 by rcammaro          #+#    #+#             */
/*   Updated: 2021/11/12 20:05:05 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBTREE_HPP
# define RBTREE_HPP

# include <iostream>
# include <string> // print function for debugging
# include <memory> // std::allocator
# include <vector.hpp>
# include <stack.hpp>
# include <cmath>

enum e_rbnode_color
{
	rbt_red = 0, rbt_black = 1
};

template <typename value_type>
struct RbNode
{
	value_type *value;
	RbNode *left, *right, *parent;
	bool color;

	RbNode(value_type *val): value(val),
	left(0), right(0), parent(0), color(rbt_red) {}
};

template <typename T, typename Compare, typename Alloc = std::allocator<T> >
class Rbtree
{
public:
	typedef	T					value_type;
	typedef	std::size_t			size_type;
	typedef	RbNode<T>			node; 
	typedef Alloc				allocator_type;

	Rbtree(Compare const &comp_obj = Compare(), allocator_type const &alloc = allocator_type());
	Rbtree(Rbtree const & src);
	Rbtree & operator=(Rbtree const & rhs);
	~Rbtree();

	node const *	insert(value_type const &value);
	node const *	insert(node * hint, value_type const &value);
	size_type		erase(value_type const &value);
	void			erase(node * target);
	void 			clear();
	node const *	find(value_type const &value) const;
	size_type		size() const;
	size_type		max_size() const;
	node const *	min() const;
	node const *	max() const;
	node const *	lower_bound(value_type const &value) const;
	node const *	upper_bound(value_type const &value) const;
	node const *	end() const;
	void			swap(Rbtree & x);
	allocator_type	get_allocator() const;

	// debug tools
	void 			print() const;
	bool			check_rb() const;
	bool			check_bst() const;

private:
	allocator_type											_value_allocator;
	typename allocator_type::template rebind<node>::other	_node_allocator;
	Compare													_comp; // templated comparision object
	node 													_end;	// stack variable
	node *													&_root;	// reference to _end.left
	size_type												_size;

	// insertion
	node *	insert(node* &root, node* parent, value_type const &value);
	void	insert_fix(node *inserted_node);
	node *	new_node(value_type const &value);
	bool	validate_hint(node const * hint, value_type const & value);
	node const *	next(node const * x) const;

	// clear
	void	clear_node(node* &x);

	// helpers and utils
	bool	is_left_child(node *x) const;
	bool	is_red(node *x) const;
	bool	is_black(node *x) const;
	void	left_rotate(node * x);
	void	right_rotate(node * x);
	node *	successor(node *x) const;
	node const *	find(node *root, value_type const &value) const;
	node *	find(node *root, value_type const &value);
	void	detach_nil_node(node & nil);
	void	transplant(node *old, node *new_node);
	void	update_end();
	void	copy_tree(node * &dest, node const *src, node *parent);
	node const * lower_bound(node const *root, value_type const &value) const;
	node const * upper_bound(node const *root, value_type const &value) const;

	// remove
	void	remove_node(node *target);
	void	fix_remove(node * deleted, node * replacement, node * x);
	node *	sibling(node *x);
	int		case_index(node *x);
	void	run_case(int index, node * x);
	void	case_1(node *x);
	void	case_2(node *x);
	void	case_3(node *x);
	void	case_4(node *x);

	// debug utils
	void	print(node* root, int space) const;
	bool	check_bst(node *root) const;
	
};


template<typename T, typename C, typename A>
Rbtree<T,C,A>::Rbtree(C const &comp_obj, allocator_type const & alloc):
_value_allocator(alloc), _comp(comp_obj), _end(_value_allocator.allocate(1)),
_root(_end.left), _size(0)
{
}

template<typename T, typename C, typename A>
Rbtree<T,C,A>::Rbtree(Rbtree const & src):
_value_allocator(src._value_allocator), 
_comp(src._comp), _end(_value_allocator.allocate(1)), _root(_end.left), _size(0)
{
	*this = src;
}

template<typename T, typename C, typename A>
Rbtree<T,C,A> & Rbtree<T,C,A>::operator=(Rbtree const & rhs)
{
	if (this != &rhs)
	{
		clear();
		copy_tree(_root, rhs._root, &_end);
		update_end();
		_size = rhs._size;
	}
	return (*this);
}

template<typename T, typename C, typename A>
void
Rbtree<T,C,A>::copy_tree(node * &dest, node const *src, node *parent)
{
	if (!src)
		dest = NULL;
	else
	{
		dest = new_node(*src->value);
		dest->parent = parent;
		dest->color = src->color;
		copy_tree(dest->left, src->left, dest);
		copy_tree(dest->right, src->right, dest);
	}
}

template<typename T, typename C, typename A>
void	Rbtree<T,C,A>::swap(Rbtree & x)
{
	// swap _roots
	_root = x._root;
	x._root = _end.right;
	update_end();
	x.update_end();

	// swap _sizes
	size_type temp_size = _size;
	_size = x._size;
	x._size = temp_size;
}

template<typename T, typename C, typename A>
Rbtree<T,C,A>::~Rbtree()
{
	clear();
	_value_allocator.deallocate(_end.value, 1);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::size_type	Rbtree<T,C,A>::size() const
{
	return (_size);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::size_type	Rbtree<T,C,A>::max_size() const
{
	return (_node_allocator.max_size() * sizeof(node)
	/ (sizeof(node) + sizeof(value_type)));
}

// whenever the root changes, _end.left holds its address, but _end.right
// needs to be updated
template<typename T, typename C, typename A>
void	Rbtree<T,C,A>::update_end()
{
	_end.right = _end.left;
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::min() const
{
	node const * ptr = &_end;
	while (ptr->left)
		ptr = ptr->left;
	return (ptr);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::max() const
{
	node const * ptr = &_end;
	while (ptr->right)
		ptr = ptr->right;
	return (ptr);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::lower_bound(node const * root, value_type const &value) const
{
	if (!root)
		return (NULL);
	if (_comp(*root->value, value)) // if node is less than value: look in right subtree
		return (lower_bound(root->right, value));
	if  (_comp(value, *root->value)) // if value is less than key: check left subtree for better candidate
	{
		node const * better_candidate = lower_bound(root->left, value);
		if (better_candidate)
			return (better_candidate);
	}
	return (root);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::upper_bound(node const * root, value_type const &value) const
{
	if (!root)
		return (NULL);
	if  (!_comp(value, *root->value)) // if node is smaller or equal to ref_value
		return (upper_bound(root->right, value)); // search in right sub-tree
	node const * better_candidate = upper_bound(root->left, value);
	if (better_candidate)
		return (better_candidate);
	return (root);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::lower_bound(value_type const &value) const
{
	node const * lb = lower_bound(_root, value);
	if (!lb)
		return (&_end);
	return (lb);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::upper_bound(value_type const &value) const
{
	node const * ub = upper_bound(_root, value);
	if (!ub)
		return (&_end);
	return (ub);
}

template<typename T, typename C, typename A>
A Rbtree<T,C,A>::get_allocator() const
{
	return (_value_allocator);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::insert(T const &value)
{
	return insert(_root, &_end, value);
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::insert(node * hint, value_type const &value)
{
	if (validate_hint(hint, value))
		return (insert(hint->right, hint, value));
	return (insert(_root, &_end, value));
}

template<typename T, typename C, typename A>
bool	Rbtree<T,C,A>::validate_hint(node const * hint, value_type const & value)
{
	if (hint == &_end || !_comp(*hint->value, value))
		return (false);
	node const * next_node = next(hint);
	if (next_node == &_end || !_comp(value, *next_node->value))
		return (false);
	return (true);
}

// returns the next node (in sorted order)
template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::next(node const * ptr) const
{	
	if (ptr->right)
	{
		ptr = ptr->right;
		while (ptr->left)
			ptr = ptr->left;
		return (ptr);
	}
	while (ptr->parent)
	{
		if (ptr->parent->left == ptr) // ptr is a left child
			return (ptr->parent);
		ptr = ptr->parent;
	}
	return (ptr); 
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node *
Rbtree<T,C,A>::insert(node* &root, node* parent, T const &value)
{
	if (!root)
	{
		// std::cout << "inserting " << value << std::endl;
		node * inserted_node = new_node(value);
		root = inserted_node;
		root->parent = parent;
		insert_fix(root);
		update_end();
		++_size;
		return inserted_node;
	}
	if (_comp(value, *root->value))
		return insert(root->left, root, value);
	if (_comp(*root->value, value))
		return insert(root->right, root, value);
	return root;
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::insert_fix(node *x)
{
	node * uncle;

	while (x != _root && is_red(x->parent))
	{
		if (is_left_child(x->parent))
		{
			uncle = x->parent->parent->right; // should be safe as it's not root or its child (parent is red, root is always black)
			if (is_red(uncle)) // case 1: change colors
			{
				 x->parent->color = rbt_black;
				 uncle->color = rbt_black;
				 x->parent->parent->color = rbt_red;
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
				x->parent->color = rbt_black;
				x->parent->parent->color = rbt_red;
				right_rotate(x->parent->parent);
			}
		}
		else // x's parent is a right child
		{
			uncle = x->parent->parent->left;
			if (is_red(uncle))
			{
				x->parent->color = rbt_black;
				uncle->color = rbt_black;
				x->parent->parent->color = rbt_red;
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
				x->parent->color = rbt_black;
				x->parent->parent->color = rbt_red;
				left_rotate(x->parent->parent);
			}
		}
	}
	// the root is always black
	_root->color = rbt_black;
}

// x cannot be null
template<typename T, typename C, typename A>
bool Rbtree<T,C,A>::is_left_child(node *x) const
{
	//////////////////////
	if (!x)
	{
		std::cout << "test for left_child with NULL pointer!\n";
		return false;
	}
	///////////////////////
	if (x->parent->left == x)
		return (true);
	return (false);
}

// x can be null
template<typename T, typename C, typename A>
bool	Rbtree<T,C,A>::is_red(node *x) const
{
	if (x && x->color == rbt_red)
		return true;
	return false;
}

// x can be null
template<typename T, typename C, typename A>
bool	Rbtree<T,C,A>::is_black(node *x) const
{
	return (!is_red(x));
}

// x->right cannot be NULL
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::left_rotate(node * x)
{
	node * y;

	y = x->right;
	// turn y's left subtree into x's right sub-tree
	x->right = y->left;
	if (y->left)
		y->left->parent = x;
	// turn x's parent into y's parent
	y->parent = x->parent;
	if (is_left_child(x))
		x->parent->left = y;
	else
		x->parent->right = y;
	// make x y's left child
	y->left = x;
	x->parent = y;
}

// x->left cannot be NULL
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::right_rotate(node *x)
{
	node * y;

	y = x->left;
	// turn y's right subtree into x's left subtree
	x->left = y->right;
	if (y->right)
		y->right->parent = x;
	// turn x's parent into y's parent
	y->parent = x->parent;
	if (is_left_child(x))
		x->parent->left = y;
	else
		x->parent->right = y;
	// make x y's right child
	y->right = x;
	x->parent = y;
}


#define INCREM 5
#define RED "\033[1;31m"
#define NC "\033[0m"
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::print(node *root, int space) const
{
	if (!root)
		return;
	print(root->right, space + INCREM);
	std::cout	<< '\n'
				<< std::string(space, ' ');
	if (root->color == rbt_red)
		std::cout << RED;
	std::cout << *root->value << NC << std::endl; 
	print(root->left, space + INCREM);
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::print() const
{
	print(_root, 0);
	std::cout << "---------------------------\n";
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node * Rbtree<T,C,A>::new_node(value_type const &value)
{
	value_type * v = _value_allocator.allocate(1);
	_value_allocator.construct(v, value);

	node * n = _node_allocator.allocate(1);
	_node_allocator.construct(n,v);

	return (n);
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::clear_node(node* &x)
{
	// destroy and deallocate value
	_value_allocator.destroy(x->value);
	_value_allocator.deallocate(x->value, 1);

	// destroy and deallocate node
	_node_allocator.destroy(x);
	_node_allocator.deallocate(x, 1);

	x = NULL;
}

// Depth first traversal using a stack (not the call stack)
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::clear()
{
	node *ptr;
	ft::vector<node *> v;
	if (_size >= 2)
		v.reserve(floor(2 * log2(_size)));
	ft::stack<node *> stack(v);

	if (_root)
		stack.push(_root);
	while (!stack.empty())
	{
		ptr = stack.top();
		stack.pop();
		if (ptr->left)
			stack.push(ptr->left);
		if (ptr->right)
			stack.push(ptr->right);
		clear_node(ptr);
	}
	_root = NULL;
	update_end();
	_size = 0;
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const *
Rbtree<T,C,A>::find(node *root, T const &value) const
{
	if (!root)
		return (&_end);
	if(_comp(value, *root->value))
		return (find(root->left, value));
	if(_comp(*root->value, value))
		return (find(root->right, value));
	return (root);
}

// this overload is necessary because erase needs a node * to remove it
template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node *
Rbtree<T,C,A>::find(node *root, T const &value) 
{
	if (!root)
		return (&_end);
	if(_comp(value, *root->value))
		return (find(root->left, value));
	if(_comp(*root->value, value))
		return (find(root->right, value));
	return (root);
}

// returns pointer to _end if no match
template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::find(T const &value) const
{
	return (find(_root, value));
}


template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node const * Rbtree<T,C,A>::end() const
{
	return (&_end);
}


template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::size_type	Rbtree<T,C,A>::erase(T const &value)
{
	node *target = find(_root, value);
	if (target == &_end)
		return (0);

	remove_node(target);
	return (1);
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::erase(node * target)
{
	if (target != &_end)
		remove_node(target);
}

template<typename T, typename C, typename A>
void	Rbtree<T,C,A>::transplant(node *old, node *new_node)
{
	// update parent
	if (old == old->parent->left) // old is a left child
		old->parent->left = new_node;
	else
		old->parent->right = new_node;
	// set new_node's pointer to parent
	new_node->parent = old->parent;
}

template<typename T, typename C, typename A>
void	Rbtree<T,C,A>::remove_node(node *target)
{
	node * replacement;
	node * x;
	node nil(NULL);

	nil.color = rbt_black;

	if (target->left && target->right) // node has two children
	{
		replacement = successor(target);
		x = replacement->right;
		if (!x)
			x = &nil;
		transplant(replacement, x); // detach replacement from tree
		transplant(target, replacement); // replace target
		replacement->left = target->left;
		replacement->left->parent = replacement;
		replacement->right = target->right;
		replacement->right->parent = replacement;
	}
	else 
	{
		if (target->left) // left child only
			replacement = x = target->left;
		else if (target->right) // right child only
			replacement = x = target->right;
		else	// no children
			replacement = x = &nil;
		transplant(target, replacement);
	}
	fix_remove(target, replacement, x);
	detach_nil_node(nil);
	clear_node(target); 
	--_size;
	update_end();
}

template<typename T, typename C, typename A>
void	Rbtree<T,C,A>::detach_nil_node(node & nil)
{
	if (nil.parent)
	{
		if (&nil == nil.parent->left)
			nil.parent->left = NULL;
		else
			nil.parent->right = NULL;
		nil.parent = NULL;
	}
}

// x->right cannot be NULL
template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node *	Rbtree<T,C,A>::successor(node *x) const
{
	x = x->right;

	while (x->left)
		x = x->left;
	return (x);
}

// Algorithm for deleting fix:
// https://github.com/alenachang/red-black-deletion-steps
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::fix_remove(node * deleted, node * replacement, node * x)
{
	if (deleted->color == rbt_red)
	{
		if (!(replacement->color == rbt_red || !replacement->value)) // second case
		{
			replacement->color = rbt_red;
			run_case(case_index(x), x);
		}
	}
	else // delete node is black
	{
		if (is_red(replacement)) // third case
			replacement->color = rbt_black;
		else if (x != _root) // fourth case
			run_case(case_index(x), x);
	}
}

template<typename T, typename C, typename A>
typename Rbtree<T,C,A>::node * Rbtree<T,C,A>::sibling(node *x)
{
	if (is_left_child(x))
		return (x->parent->right);
	return (x->parent->left);
}

template<typename T, typename C, typename A>
int Rbtree<T,C,A>::case_index(node *x)
{
	if (x->color == rbt_red)
		return (0);
	// x is black
	node * w = sibling(x);
	if (w->color == rbt_red)
		return (1);
	// w is black
	if (is_black(w->left) && is_black(w->right))
		return (2);
	// at lest one of w's children is red
	if (is_left_child(x))
	{
		if (is_black(w->right))
			return (3);
		return (4);
	}
	else // x is right_child
	{
		if (is_black(w->left))
			return (3);
		return (4);
	}
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::run_case(int index, node * x)
{
	if (index == 0)
		x->color = rbt_black;
	else if (index == 1)
		case_1(x);
	else if (index == 2)
		case_2(x);
	else if (index == 3)
		case_3(x);
	else
		case_4(x);
}

// 	Case 1: Node x is black and its sibling w is red
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::case_1(node *x)
{
	node * w = sibling(x);

	w->color = rbt_black;
	x->parent->color = rbt_red;
	if (is_left_child(x))
		left_rotate(x->parent);
	else
		right_rotate(x->parent);
	
	int case_i = case_index(x);
	run_case(case_i, x);
}

// Node x is black and its sibling w is black and both of w's children
// are black
template<typename T, typename C, typename A>
void Rbtree<T,C,A>::case_2(node *x)
{
	node * w = sibling(x);
	w->color = rbt_red;
	x = x->parent;
	if (is_red(x))
		x->color = rbt_black;
	else if (x != _root)
		run_case(case_index(x), x);
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::case_3(node *x)
{
	node * w = sibling(x);

	w->color = rbt_red;

	if (is_left_child(x))
	{
		w->left->color = rbt_black;
		right_rotate(w);
	}
	else
	{
		w->right->color = rbt_black;
		left_rotate(w);
	}
	run_case(4, x);
}

template<typename T, typename C, typename A>
void Rbtree<T,C,A>::case_4(node *x)
{
	node * w = sibling(x);
	
	w->color = x->parent->color;
	x->parent->color = rbt_black;
	if (is_left_child(x))
	{
		w->right->color = rbt_black;
		left_rotate(x->parent);
	}
	else
	{
		w->left->color = rbt_black;
		right_rotate(x->parent);
	}
}

////////// debug Utils

int max (int a, int b)
{
	return (a > b ? a : b);
}

int min (int a, int b)
{
	return (a < b ? a : b);
}

template<typename T>
int max_black_depth(RbNode<T> *root)
{
	if (!root)
		return (0);
	return (root->color +
			max(max_black_depth(root->left), max_black_depth(root->right)));
}

template<typename T>
int min_black_depth(RbNode<T> *root)
{
	if (!root)
		return (0);
	return (root->color +
			min(min_black_depth(root->left), min_black_depth(root->right)));
}

template<typename T>
bool check_reds(RbNode<T> *root, int parent_color)
{
	int node_color;

	if (!root)
		return true;
	node_color = root->color;
	if (node_color == rbt_red && parent_color == rbt_red)
		return false;
	return (check_reds(root->left, node_color)
			&& check_reds(root->right, node_color));
}

template<typename T, typename C, typename A>
bool Rbtree<T,C,A>::check_bst(node *root) const
{
	if (!root)
		return true;
	if (root->left && !_comp(*root->left->value, *root->value))
		return false;
	if (root->right && !_comp(*root->value, *root->right->value))
		return false;
	return (check_bst(root->left) && check_bst(root->right));
}

template<typename T, typename C, typename A>
bool Rbtree<T,C,A>::check_bst() const
{
	return (check_bst(_root));
}

template<typename T, typename C, typename A>
bool Rbtree<T,C,A>::check_rb() const
{
	bool black_depth = true;
	bool consec_red = true;

	if (max_black_depth<T>(_root) != min_black_depth<T>(_root))
	{
		std::cout << "check_rb failed: black depth not uniform" << std::endl;
		black_depth = false;
	}
	if (!check_reds<T>(_root, rbt_red))
	{
		std::cout << "check_rb failed: consecutive reds" << std::endl;
		consec_red = false;
	}
	return (black_depth && consec_red);
}

#endif