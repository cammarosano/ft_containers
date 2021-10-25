#include <criterion/criterion.h>
#include "Rbtree.hpp"
#include "tree_iterator.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

struct CompInt // key comparision
{
	bool operator()(int const & a, int b) const
	{
		return (a < b);
	}
};

struct CompValue // value comparision
{
	CompInt key_comp;

	bool operator()(int const &a, int const &b) const
	{
		return (key_comp(a, b)); // shall be a.first in case of pairs
	}

};

Rbtree<int, CompValue> t;

typedef RbNode<int> node;

// int main()
void setup()
{
	t.insert(5);
	t.insert(10);
	t.insert(15);
	t.insert(10);
	t.insert(11);
	t.insert(0);
	t.insert(1);
	t.insert(3);
	t.insert(22);
	t.insert(24);
	t.insert(26);
	t.insert(28);
}

Test(rb_check, rb_check, .init=setup)
{
	cr_expect(t.check_rb());
	node *x = t.end()->left; // root
	x = x->left;
	x->color = rbt_red;
	// cr_expect(!t.check_rb());
}

Test(find, test, .init=setup)
{
	cr_expect(t.find(5) != t.end());
	cr_expect(t.find(3) != t.end());
	cr_expect(t.find(50) == t.end());

}

Test(remove, leaf_node, .init=setup)
{

	cr_expect(t.erase(28));
	cr_expect(!t.erase(50));
}

Test(remove, one_child_node, .init=setup)
{
	cr_expect(t.erase(26));
	cr_expect(t.erase(5));
}

Test(remove, two_children_node, .init=setup)
{
	cr_expect(t.erase(24));
	cr_expect(t.erase(1));
	cr_expect(t.erase(10)); // the root
	cr_expect(t.find(28));
}

Test(remove, remove_fix, .init=setup)
{
	cr_expect(t.erase(28));
	cr_expect(t.erase(26));
	cr_expect(t.erase(11));
	cr_expect(t.erase(10)); // root
}

void infix_add2vector(node *root, std::vector<int> &v)
{
	if (!root)
		return ;
	infix_add2vector(root->left, v);
	v.push_back(*root->value);
	infix_add2vector(root->right, v);
}

std::vector<int> btree2vector(Rbtree<int,CompValue> &t)
{
	std::vector<int> v;
	node * root = ((t.end())->left);
	// node * root = const_cast<node *>((t.getEnd())->left);
	infix_add2vector(root, v);
	return (v);
}


Test(remove, random)
{
	int const n = 1000;
	int index;
	int	new_nb;
	int delete_count = 0;
	int insert_count = 0;
	size_t tree_size;

	std::srand(std::time(NULL));

	for (int i = 0; i < n; i++)
	{
		t.insert(std::rand() % n);
		cr_expect(t.check_rb());
	}
	
	std::vector<int> v = btree2vector(t);
	
	while (v.size())
	{
		index = std::rand() % v.size();
		delete_count += t.erase(v[index]);
		v.erase(v.begin() + index);
		cr_expect(t.check_rb());
		if (index % 2)
		{
			new_nb = std::rand() % n;
			tree_size = t.size();
			t.insert(new_nb);
			insert_count += t.size() > tree_size;
			cr_expect(t.check_rb());
			v.push_back(new_nb);
		}
	}
	std::cout << "remove test with randon numbers: "
		<< delete_count << " deletions and "
		<< insert_count << " insertions" << std::endl;

}

Test(min, test, .init=setup)
{
	cr_expect(*t.min()->value == 0);
	t.clear();
	cr_expect(t.size() == 0);
	cr_expect(t.min() == t.end());
}

Test(max, test, .init=setup)
{
	cr_expect(*t.max()->value == 28);
	t.clear();
	cr_expect(t.max() == t.end());
}

Test(copy_construction, test, .init=setup)
{
	Rbtree<int, CompValue> t2(t);
	cr_expect(*t2.min()->value == *t.min()->value);
	cr_expect(*t2.max()->value == *t.max()->value);
	cr_expect(t2.size() == t.size());

	Rbtree<int, CompValue> t3;
	t3.insert(3);
	t3.insert(5);
	t3 = t2;
	cr_expect(*t2.min()->value == *t3.min()->value);
	cr_expect(*t2.max()->value == *t3.max()->value);
	cr_expect(t2.size() == t3.size());
}

Test(tree_iterator, test, .init=setup)
{
	ft::tree_iterator<int> it(t.min());
	ft::tree_iterator<int> ite(t.end());
	cr_expect(*it == 0);
	while (it != ite)
	{
		// std::cout << *it << std::endl;
		++it;
	}
	--it;
	cr_expect(*it == 28);
}

Test(max_size, test)
{
	cr_expect(t.max_size());
}

Test(insert_hint, test, .init=setup)
{
	RbNode<int> const * hint = t.find(22);
	t.insert(const_cast<RbNode<int>*>(hint), 23);
}

Test(swap, test, .init=setup)
{
	Rbtree<int, CompValue> t2;
	t2.insert(19);
	t2.insert(42);
	t2.insert(21);
	t.swap(t2);
	cr_expect(t.size() == 3);
}

Test (lower_upper_bound, test, .init=setup)
{
	cr_expect(*t.lower_bound(11)->value == 11);
	cr_expect(*t.upper_bound(11)->value == 15);
	cr_expect(t.lower_bound(29) == t.end());
	cr_expect(t.upper_bound(29) == t.end());
}

