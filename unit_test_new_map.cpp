#include <criterion/criterion.h>
#include <iostream>
#include "ft_map.hpp"


ft::map<int,std::string> m;
ft::map<int,std::string>::iterator it;

void setup()
{
	m[0] = "zero";
	m[2] = "two";
	m[4] = "four";
	m[6] = "six";
	m[8] = "eight";
	m[10] = "ten";
}

Test(constructor, copy, .init=setup)
{
	ft::map<int,std::string> m2(m);
	cr_expect(m2.size() == m.size());
	cr_expect(m2.begin()->first == m.begin()->first);
	cr_expect((--(m2.end()))->first == (--(m.end()))->first);
}

bool comp_func(int a, int b)
{
	return (a > b);
}

Test(constructor, pointer2function)
{
	ft::map<int,int,bool (*)(int,int)> m(comp_func);
	m[1] = 1;
	m[2] = 1;
	m[3] = 1;
	m[4] = 1;
	cr_expect(m.begin()->first == 4);
}

Test(capacity, empty)
{
	cr_expect(m.empty());
	m[1] = "";
	cr_expect(!m.empty());
}

Test(capacity, size)
{
	cr_expect(m.size() == 0);
	m[1] = "";
	cr_expect(m.size() == 1);
	m[1] = "hello";
	cr_expect(m.size() == 1);
	m[2] = "hello again";
	cr_expect(m.size() == 2);
}

Test(bracket_operator, test)
{
	m[5] = "hello";
	cr_expect(m[5] == "hello");
	cr_expect(m[10] == "");
	cr_expect(m.size() == 2);
}

Test(insert, key)
{

	cr_expect(m.insert(ft::make_pair(19, "yo")).second);
	cr_expect(m.insert(ft::make_pair(21, "yo")).second);
	cr_expect(!m.insert(ft::make_pair(21, "yo")).second);
	cr_expect(m.insert(ft::make_pair(15, "yo")).first->first == 15);
}

Test(iterators, begin, .init=setup)
{
	it = m.begin();
	cr_expect(it->first == 0);
	size_t i = 0;
	while (it != m.end())
	{
		++it;
		++i;
	}
	cr_expect(m.size() == i);
}

Test(const_iterator, begin, .init=setup)
{
	// todo: make a copy constructor, instantiate const map by copy, get iterators
	ft::map<int,std::string> const m2(m);

	ft::map<int,std::string>::const_iterator cit = m2.begin();

	std::cout << cit->first << std::endl;

}
