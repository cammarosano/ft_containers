#include <criterion/criterion.h>
#include <iostream>
#include "ft_set.hpp"
#include <set>
#include <vector>

ft::set<int> s;
ft::set<int>::iterator it;
std::set<int> stl_set;

void setup()
{
	s.insert(5);
	s.insert(25);
	s.insert(10);
	s.insert(20);
	s.insert(15);
}

Test(iterator, begin, .init=setup)
{
	it = s.begin();
	cr_expect(*it == 5);
	while (it != s.end())
		++it;
	--it;
	cr_expect(*it == 25);
}

Test(insert, hint, .init=setup)
{
	ft::set<int>::iterator ret;

	it = s.begin();
	++it; // 10
	ret = s.insert(it, 12); // good hint
	cr_expect(*ret == 12);
	++it; // 12
	ret = s.insert(it, 8); // bad hint
	cr_expect(*ret == 8);
	cr_expect(s.size() == 7);
}

Test(insert, range, .init=setup)
{
	std::vector<int> v;
	v.push_back(10); // set already contains 10
	v.push_back(11);
	v.push_back(12);
	v.push_back(13);
	s.insert(v.begin(), v.end());
	// s.print_tree();
	cr_expect(s.size() == 8);
}

Test(capacity, test, .init=setup)
{
	cr_expect(!s.empty());
	cr_expect(s.size() == 5);
	// std::cout << s.max_size() << std::endl;
	// std::cout << stl_set.max_size() << std::endl;
}