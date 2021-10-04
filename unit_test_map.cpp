#include <criterion/criterion.h>
#include <iostream>
#include "ft_map.hpp"
#include <string>


ft::map<int, std::string> m;

void setup(void)
{
	m[1] = "one";
	m[0] = "zero";
	m[2] = "two";
	m[3] = "three";
}

Test(constructor, default_construct)
{
	ft::map<int, std::string> m;
	cr_expect(m.size() == 0);
}

Test(pair, constructor)
{
	ft::pair<int,std::string> pr;

	pr.first = 1;
	pr.second = "hello";

	// std::cout << pr.first << " - " << pr.second << std::endl;
	cr_expect(pr.first == 1 && pr.second == "hello");

	ft::pair<int,std::string> pr2(2, "again");
	// std::cout << pr2.first << " - " << pr2.second << std::endl;
	cr_expect(pr2.first == 2 && pr2.second == "again");

	ft::pair<int,std::string> pr3(pr);
	cr_expect(pr3.first == 1 && pr3.second == "hello");

}

Test(insert, insert_pair)
{
	ft::map<int, std::string> m;
	ft::pair<ft::map<int, std::string>::iterator, bool> ret;

	ft::pair<int, std::string> 	pr(42, "forty-two");
	ret = m.insert(pr);
	cr_expect(ret.second);

	ret = m.insert(pr); // same key
	cr_expect(ret.second == false);
	cr_expect(m.size() == 1);

	ret = m.insert(ft::pair<int, std::string>(19, "nineteen"));
	cr_expect(ret.second);
	cr_expect(m.size() == 2);

	ret = m.insert(ft::pair<int, std::string>(21, "blackjack"));
	cr_expect(ret.second);
	cr_expect(m.size() == 3);

	ret = m.insert(ft::map<int, std::string>::value_type(66, "the beast!"));
	cr_expect(ret.second);
	cr_expect(m.size() == 4);

	// ft::map<int, std::string>::iterator it = ret.first;
	// std::cout << it->first << std::endl;
	// std::cout << it->second << std::endl;

	m.clear();
	cr_expect(m.size() == 0);

}

Test(operator_brackets, test)
{
	ft::map<std::string, int> m;

	m["hello"] = 42;
	cr_expect(m["hello"] == 42);
	m["you"] = 19;
	cr_expect(m["you"] == 19);
	cr_expect(m["new"] == 0);
	m["yey"] = m["hello"];
	cr_expect(m["yey"] == m["hello"]);

	// std::cout << m["hello"] << std::endl;
	// std::cout << m["you"] << std::endl;
	// std::cout << m["new"] << std::endl;
	// std::cout << m["new_again"] << std::endl;
}

Test(iterator, pre_increment, .init=setup)
{
	ft::map<int, std::string>::iterator it;

	it = m.begin();
	cr_expect(it->first == 0);
	cr_expect(it->second == "zero");

	++it;
	cr_expect(it->first == 1);
	cr_expect(it->second == "one");

	++it;
	cr_expect(it->first == 2);
	cr_expect(it->second == "two");
	cr_expect(it->second == m[2]);

	++it;
	cr_expect(it->first == 3);
	cr_expect(it->second == "three");
}

Test(iterator, equalitiy, .init=setup)
{
	ft::map<int, std::string>::iterator it;
	it = m.begin();
	while (it != m.end())
	{
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}
	
}