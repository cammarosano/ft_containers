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
	CompValue		_comp;
	node 			_end;	// stack variable
	node *			&_root;	// reference to _end.left
	node			_nil;
	unsigned long	_size;

	void	update_end();
	bool	insert(node* &root, node* parent, int const &value);
	void	insert_fix(node *inserted_node);
	node *	new_node(int const &value);
	void	delete_node(node* &x);
	void	clear(node* &root);
	void	print(node* root, int space) const;
	bool	is_left_child(node *x) const;
	bool	is_red(node *x) const;
	void	left_rotate(node * x);
	void	right_rotate(node * x);
	node*	find(node *root, int const &value);

public:
	Rbtree();
	~Rbtree();

	void	insert(int const &value); 
	void 	clear();
	void 	print() const;
	node*	find(int const &value);

	node const *	getEnd() const; // debug only
};





















Rbtree::Rbtree():_end(0), _root(_end.left), _nil(0), _size(0)
{
}

Rbtree::~Rbtree()
{
	clear();
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
}

node * Rbtree::new_node(int const &value)
{
	int * content = new int(value);

	return (new node(content));
}

void Rbtree::delete_node(node* &x)
{
	delete x->value;
	delete x;
	x = NULL;
}

void Rbtree::clear(node* &root)
{
	if (root)
	{
		clear(root->left);
		clear(root->right);
		delete_node(root);
	}
}

void Rbtree::clear()
{
	clear(_root);
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

#endif