#include <criterion/criterion.h>
#include "ft_stack.hpp"
#include <iostream>
#include <stack>
#include <vector>

Test(constructor, test)
{
	ft::stack<int> stk;

	stk.push(5);
	stk.push(10);
	cr_expect(stk.size() == 2);
	// std::cout << stk.top() << std::endl;
	stk.pop();
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