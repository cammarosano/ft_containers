#include "Rbtree.hpp"

struct CompInt // key comparision
{
	bool operator()(int const & a, int b)
	{
		return (a < b);
	}
};

int main()
{
	Rbtree<int, CompInt> t;

	t.insert(5);
	t.insert(10);
	t.insert(0);
	t.print();
	RbNode<int> const * p = t.min();
	*p->value = 50; // chaging what p->value points to, but not value (a pointer) itself
	t.print();

}