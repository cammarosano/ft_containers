#include <criterion/criterion.h>
#include "Rbtree.hpp"

Rbtree t;

// int main()
Test(tree, test)
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
	// t.print();

	cr_expect(t.find(5) != t.getEnd());
	cr_expect(t.find(3) != t.getEnd());
	cr_expect(t.find(50) == t.getEnd());

}
