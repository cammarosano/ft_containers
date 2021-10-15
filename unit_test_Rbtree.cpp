#include <criterion/criterion.h>
#include "Rbtree.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

Rbtree t;

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

Test(find, test, .init=setup)
{
	cr_expect(t.find(5) != t.getEnd());
	cr_expect(t.find(3) != t.getEnd());
	cr_expect(t.find(50) == t.getEnd());

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

std::vector<int> btree2vector(Rbtree &t)
{
	std::vector<int> v;
	node * root = ((t.getEnd())->left);
	// node * root = const_cast<node *>((t.getEnd())->left);
	infix_add2vector(root, v);
	return (v);
}


Test(remove, random)
{
	Rbtree tree;
	int const n = 1000;
	int index;
	int	new_nb;

	std::srand(std::time(NULL));
	for (int i = 0; i < n; i++)
		tree.insert(std::rand() % n);
	
	std::vector<int> v = btree2vector(tree);
	// for (size_t i = 0; i < v.size(); i++)
	// {
	// 	std::cout << v[i] << ' ';
	// }
	
	while (v.size())
	{
		index = std::rand() % v.size();
		tree.erase(v[index]);
		v.erase(v.begin() + index);
		if (index % 2)
		{
			new_nb = std::rand() % n;
			tree.insert(new_nb);
			v.push_back(new_nb);
		}
	}

}