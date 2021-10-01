#include "ft_vector.hpp"
#include <iostream>
#include <vector>

class Foo
{
	int * m_a;

public:
	Foo(int * a): m_a(a) {}

	void setA(int * val)
	{
		m_a = val;
	}
	int * getA(void) const
	{
		return (m_a);
	}
};

int main()
{
	// int x = 5;
	// Foo const foo(&x);

	// int *ptr = foo.getA();
	// *ptr = 42;

	// std::cout << *(foo.getA()) << std::endl;
	
	std::vector<int> const v (5, 42);

	std::vector<int>::const_iterator it = v.begin();
	// *it = 43;

	ft::vector<int> const my_v (5, 42);

	ft::vector<int>::const_iterator my_it = my_v.begin();
	std::cout << *my_it << std::endl;
	// *my_it = 43;

	std::vector<int> v2(9, 19);	// non const
	
	std::vector<int>::iterator itv2 = v2.begin();

	std::vector<int>::const_iterator cit(itv2); // contruct from normal iterator
	std::cout << *cit << std::endl;

	// std::vector<int>::iterator normal_it(cit); // the opposite can't be done



}