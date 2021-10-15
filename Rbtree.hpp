#ifndef RBTREE_HPP
# define RBTREE_HPP

# include <iostream>
# include <string>

enum e_rbnode_color
{
	rbt_red = 0, rbt_black = 1
};

struct CompInt // key comparision
{
	bool operator()(int const & a, int b)
	{
		return (a < b);
	}
};

struct CompValue // value comparision
{
	CompInt key_comp;

	bool operator()(int const &a, int const &b)
	{
		return (key_comp(a, b)); // shall be a.first in case of pairs
	}

};

struct node
{
	int	 *value;
	node *left, *right, *parent;
	bool color;

	node(int *val): value(val), left(0), right(0), parent(0), color(rbt_red)
	{
	}
};

class Rbtree
{
private:
	typedef	std::size_t size_type;

	CompValue		_comp;
	node 			_end;	// stack variable
	node *			&_root;	// reference to _end.left
	node			_nil;
	size_type		_size;

	void	update_end();
	bool	insert(node* &root, node* parent, int const &value);
	void	insert_fix(node *inserted_node);
	node *	new_node(int const &value);
	void	clear_node(node* &x);
	void	clear_tree(node* &root);
	void	print(node* root, int space) const;
	bool	is_left_child(node *x) const;
	bool	is_red(node *x) const;
	bool	is_black(node *x) const;
	void	left_rotate(node * x);
	void	right_rotate(node * x);
	node*	find(node *root, int const &value);
	void	remove_node(node *target);
	void	transplant(node *old, node *new_node);
	void	detach_nil_node();
	node *	successor(node *x) const;

	// fix_remove
	void	fix_remove(node * deleted, node * replacement, node * x);
	node *	sibling(node *x);
	int		case_index(node *x);
	void	run_case(int index, node * x);
	void	case_1(node *x);
	void	case_2(node *x);
	void	case_3(node *x);
	void	case_4(node *x);


public:
	Rbtree();
	~Rbtree();

	void		insert(int const &value); 
	size_type	erase(int const &value);
	void 		clear();
	void 		print() const;
	node*		find(int const &value);

	node const *	getEnd() const; // debug only
	size_type		size() const;
};





















Rbtree::Rbtree():_end(0), _root(_end.left), _nil(0), _size(0)
{
	_nil.color = rbt_black;
}

Rbtree::~Rbtree()
{
	clear();
}

Rbtree::size_type	Rbtree::size() const
{
	return (_size);
}

// whenever the root changes, _end.left holds its address, but _end.righ
// needs to be updated
void	Rbtree::update_end()
{
	_end.right = _end.left;
}

void Rbtree::insert(int const &value)
{
	insert(_root, &_end, value);
}

bool Rbtree::insert(node* &root, node* parent, int const &value)
{
	if (!root)
	{
		std::cout << "inserting " << value << std::endl;
		root = new_node(value);
		root->parent = parent;
		insert_fix(root);
		update_end();
		++_size;
		return true;
	}
	if (_comp(value, *root->value))
		return insert(root->left, root, value);
	if (_comp(*root->value, value))
		return insert(root->right, root, value);
	return false;
}

void Rbtree::insert_fix(node *x)
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
bool Rbtree::is_left_child(node *x) const
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
bool	Rbtree::is_red(node *x) const
{
	if (x && x->color == rbt_red)
		return true;
	return false;
}

// x can be null
bool	Rbtree::is_black(node *x) const
{
	return (!is_red(x));
}

// x->right cannot be NULL
void Rbtree::left_rotate(node * x)
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
	if (is_left_child(x))
		x->parent->left = y;
	else
		x->parent->right = y;
	// make x y's left child
	y->left = x;
	x->parent = y;
}

// x->left cannot be NULL
void Rbtree::right_rotate(node *x)
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
void Rbtree::print(node *root, int space) const
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

void Rbtree::print() const
{
	print(_root, 0);
	std::cout << "---------------------------\n";
}

node * Rbtree::new_node(int const &value)
{
	int * content = new int(value);

	return (new node(content));
}

void Rbtree::clear_node(node* &x)
{
	delete x->value;
	delete x;
	x = NULL;
}

void Rbtree::clear_tree(node* &root)
{
	if (root)
	{
		clear_tree(root->left);
		clear_tree(root->right);
		clear_node(root);
	}
}

void Rbtree::clear()
{
	clear_tree(_root);
	update_end();
	_size = 0;
}


node* Rbtree::find(node *root, int const &value)
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
node* Rbtree::find(int const &value)
{
	return (find(_root, value));
}


node const * Rbtree::getEnd() const
{
	return (&_end);
}


Rbtree::size_type	Rbtree::erase(int const &value)
{
	node *target = find(value);
	if (target == &_end)
		return (0);

	std::cout << "deleting " << value << std::endl;
	remove_node(target);
	return (1);
}

void	Rbtree::transplant(node *old, node *new_node)
{
	// update parent
	if (old == old->parent->left) // old is a left child
		old->parent->left = new_node;
	else
		old->parent->right = new_node;
	// set new_node's pointer to parent
	new_node->parent = old->parent;
}

void	Rbtree::remove_node(node *target)
{
	node * replacement;
	node * x;

	if (target->left && target->right) // node has two children
	{
		replacement = successor(target);
		x = replacement->right;
		if (!x)
			x = &_nil;
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
			replacement = x = &_nil;
		transplant(target, replacement);
	}
	fix_remove(target, replacement, x);
	detach_nil_node();
	clear_node(target); 
	--_size;
	update_end();
}

void	Rbtree::detach_nil_node()
{
	if (_nil.parent)
	{
		if (&_nil == _nil.parent->left)
			_nil.parent->left = NULL;
		else
			_nil.parent->right = NULL;
		_nil.parent = NULL;
	}
}

// x->right cannot be NULL
node *	Rbtree::successor(node *x) const
{
	x = x->right;

	while (x->left)
		x = x->left;
	return (x);
}

void Rbtree::fix_remove(node * deleted, node * replacement, node * x)
{
	if (deleted->color == rbt_red)
	{
		if (replacement->color == rbt_red || replacement == &_nil) // first case
		{
			// std::cout << "initial step # 1 \n";
			return ;
		}
		// replacement is black - second case
		replacement->color = rbt_red;
		// std::cout << "initial step # 2 \n";
		run_case(case_index(x), x);
	}
	else // delete node is black
	{
		if (is_red(replacement)) // third case
		{
			// std::cout << "initial step # 3 \n";
			replacement->color = rbt_black;
		}
		else if (x != _root) // fourth case
		{
			// std::cout << "initial step # 4 \n";
			run_case(case_index(x), x);
		}
		// fifth case: x is the root, we are done.
		// else
			// std::cout << "initial step # 5 \n";
	}
}

node * Rbtree::sibling(node *x)
{
	if (x == _root) // debug only
	{
		std::cout << "sibling of root attempt ... " << std::endl;
		return (NULL);
	}
	if (is_left_child(x))
		return (x->parent->right);
	return (x->parent->left);
}

int Rbtree::case_index(node *x)
{
	if (x->color == rbt_red)
		return (0);
	// x is black
	node * w = sibling(x);
	if (!w) // debug
	{
		std::cout << "sibling is NULL!" << std::endl;
		return (-1);
	}
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

void Rbtree::run_case(int index, node * x)
{
	if (index < 0 || index > 4) // debug
	{
		std::cout << "bad index" << std::endl;
		return ;
	}
	// std::cout << "case " << index << "\n";

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
void Rbtree::case_1(node *x)
{
	node * w = sibling(x);

	// 1. Color w black
	w->color = rbt_black;

	// 2. Color x.p red
	x->parent->color = rbt_red;

	// 3. Rotate x.p
	// a. If x is the left child do a left rotation
	if (is_left_child(x))
		left_rotate(x->parent);
	// b. If x is the right child do a right rotation
	else
		right_rotate(x->parent);

	// 4. Now we have to change w
	// a. If x is the left child set w = x.p.right
	// b. If x is the right child set w = x.p.left
	// 5. With x and our new w, decide on case 2, 3, or 4 from here.
	int case_i = case_index(x);
	if (case_i < 2)
	{
		std::cout << "case index should be 2, 3 or 4" << std::endl;
		return ;
	}
	run_case(case_i, x);
}

// Node x is black and its sibling w is black and both of w's children
// are black
void Rbtree::case_2(node *x)
{
	node * w = sibling(x);
	w->color = rbt_red;
	x = x->parent;
	if (is_red(x))
		x->color = rbt_black;
	else if (x != _root)
		run_case(case_index(x), x);
}

void Rbtree::case_3(node *x)
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

void Rbtree::case_4(node *x)
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

#endif