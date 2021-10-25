#include <criterion/criterion.h>
#include "stack.hpp"
#include <iostream>
#include <stack>
#include <vector>
#include "vector.hpp"

ft::stack<int> stk;

Test(constructor, test)
{

	stk.push(5);
	stk.push(10);
	cr_expect(stk.size() == 2);
	cr_expect(stk.top() == 10);
	// std::cout << stk.top() << std::endl;
	stk.pop();
	cr_expect(stk.top() == 5);
	cr_expect(stk.size() == 1);
	cr_expect(!stk.empty());
	// std::cout << stk.top() << std::endl;
	stk.pop();
	cr_expect(stk.size() == 0);
	cr_expect(stk.empty());
	stk.pop();
	cr_expect(stk.size() == 0);
	stk.pop();
	cr_expect(stk.size() == 0);
	stk.pop();
	stk.pop();
	// std::cout << stk.top() << std::endl; // undefined behavior for STL as well

}

Test(STL_top, test)
{
	std::stack<int, std::vector<int> > stl_stack;

	stl_stack.push(5);
	stl_stack.push(10);
	stl_stack.pop();
	stl_stack.pop();
	// std::cout << stl_stack.top() << std::endl;
}

Test(relational_operators, test)
{
	stk.push(2);
	stk.push(4);
	stk.push(6);
	stk.push(8);

	ft::stack<int> stk2(stk);

	cr_expect(stk2 == stk);
	stk.push(10);
	cr_expect(!(stk2 == stk));
	cr_expect(stk2 != stk);
}

