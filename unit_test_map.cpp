#include <criterion/criterion.h>
#include <iostream>
#include "ft_map.hpp"
#include <string>
#include <vector>


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
		// std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}
	// dereferencing iterator to end
	// std::cout << it->first << ": " << it->second << std::endl;
	// std::cout << &(*it) << std::endl;
	cr_expect(it == m.end());
}

Test(iterator, end, .init=setup)
{
	ft::map<int, std::string>::iterator it;
	it = m.begin();
	size_t n = 0;
	while (it++ != m.end())
		++n;
	cr_expect(m.size() == n);
}

Test(iterador, bidirectional_it, .init=setup)
{
	ft::map<int, std::string>::iterator it1 = m.begin();
	ft::map<int, std::string>::iterator it2(it1);

	while (it1 != m.end())
	{
		cr_expect(it1++ == it2);
		++it2;
	}
	--it1;
	it2--;
	cr_expect(it1 == it2);
	cr_expect(it1->first == 3);

	it2->second = "THREE";
	cr_expect(it1->second == "THREE");
	cr_expect(m[3] == "THREE");
}

Test(find, key, .init=setup)
{
	ft::map<int, std::string>::iterator it;

	for (int i = 0; i < static_cast<int>(m.size()); i++)
	{
		it = m.find(i);
		// std::cout << it->second << std::endl;
		cr_expect(it->first == i);
	}
	it = m.find(5);
	cr_expect(it == m.end());
	
}

Test(count, key, .init=setup)
{
	cr_expect(m.count(0) == 1);
	cr_expect(m.count(1) == 1);
	cr_expect(m.count(2) == 1);
	cr_expect(m.count(3) == 1);
	cr_expect(m.count(4) == 0);
}

Test(erase, no_children, .init=setup)
{
	cr_expect(m.erase(4) == 0);
	cr_expect(m.erase(0) == 1);
	cr_expect(m.size() == 3);
	cr_expect(m.count(0) == 0);
	cr_expect(m.count(3) == 1);
	cr_expect(m.erase(3) == 1);
	cr_expect(m.size() == 2);
	cr_expect(m.count(3) == 0);
	cr_expect(m.erase(2) == 1);
	cr_expect(m.count(2) == 0);
	cr_expect(m.erase(1) == 1); // root
	cr_expect(m.count(1) == 0);
	cr_expect(m.size() == 0);
}

Test(erase, one_child, .init=setup)
{
	ft::map<int, std::string>::iterator it = m.begin();

	cr_expect(m.count(2) == 1);
	cr_expect(m.erase(2) == 1);
	cr_expect(m.count(2) == 0);

	while (it != m.end())
		++it; // no crash

	m.erase(3);

	cr_expect(m.count(1) == 1);
	cr_expect(m.erase(1) == 1);
	cr_expect(m.count(1) == 0);
	cr_expect(m.count(0) == 1);
}

// TODO: test erase two children
Test(erase, two_children, .init=setup)
{
	ft::map<int, std::string>::iterator it = m.begin();
	cr_expect(m.erase(1) == 1);
	cr_expect(m.count(1) == 0);
	cr_expect(m.count(0) == 1);
	cr_expect(m.count(2) == 1);
	cr_expect(m.count(3) == 1);
	cr_expect(m.size() == 3);
	
	cr_expect(it->first == 0);
	++it;
	cr_expect(it->first == 2);
	++it;
	cr_expect(it->first == 3);
	++it;
	cr_expect(it == m.end());

}

Test(erase, iterator, .init=setup)
{
	ft::map<int, std::string>::iterator it = m.begin();
	m.erase(it);
	cr_expect(m.count(0) == 0);

}

Test(erase, range, .init=setup)
{
	ft::map<int, std::string>::iterator it, ite;
	
	it = m.begin();
	ite = m.end();
	
	m.erase(it, --ite);
	cr_expect(m.size() == 1);
	cr_expect(m.count(3) == 1);
}

Test(erase, range_total, .init=setup)
{
	ft::map<int, std::string>::iterator it, ite;
	
	it = m.begin();
	ite = m.end();
	
	m.erase(it, ite);
	cr_expect(m.size() == 0);
}

Test(make_pair, test)
{
	ft::map<std::string, double> m;
	m.insert(ft::make_pair("hello", 0.5));
	m.insert(ft::make_pair("bye", 666));
	ft::map<std::string, double>::iterator it = m.begin();
	cr_expect(it->first == "bye");
	++it;
	cr_expect(it->second == 0.5f);
}

Test(insert, hint, .init=setup)
{
	ft::map<int, std::string> m;
	ft::map<int, std::string>::iterator it, ite, ret;
	
	m.insert(ft::make_pair(10, ""));
	m.insert(ft::make_pair(5, ""));
	m.insert(ft::make_pair(15, ""));
	m.insert(ft::make_pair(8, ""));
	m.insert(ft::make_pair(0, ""));

	it = m.begin(); // 0

	ret = m.insert(it, ft::make_pair(2, "")); // good hint
	m.insert(ret, ft::make_pair(3, "")); // good hint
	m.insert(it, ft::make_pair(4, "")); // bad hint

	cr_expect(m.size() == 8);
	it = m.begin();
	ite = m.end();
	ite--;
	while (it != ite)
	{
		cr_expect(it->first < (++it)->first);
	}
}

Test(insert, range, .init=setup)
{
	std::vector<ft::pair<int, std::string>> v;

	v.push_back(ft::make_pair(0, ""));
	v.push_back(ft::make_pair(2, ""));
	v.push_back(ft::make_pair(4, ""));
	v.push_back(ft::make_pair(6, ""));
	v.push_back(ft::make_pair(8, ""));

	m.insert(v.begin(), v.end());
	cr_expect(m.size() == 7);

}

// TODO: test swap, lower_bound
// consider making _end const (and not swaping it)
// consider not having a _root

Test(lower_and_upper_bound, test, .init=setup)
{
	ft::map<int, std::string>::iterator it;

	int key;

	key = 0;
	it = m.lower_bound(key);
	cr_expect(it->first == 0);
	it = m.upper_bound(key);
	cr_expect(it->first == 1);

	key = 1;
	it = m.lower_bound(key);
	cr_expect(it->first == 1);
	it = m.upper_bound(key);
	cr_expect(it->first == 2);

	key = 2;
	it = m.lower_bound(key);
	cr_expect(it->first == 2);
	it = m.upper_bound(key);
	cr_expect(it->first == 3);

	key = 3;
	it = m.lower_bound(key);
	cr_expect(it->first == 3);
	it = m.upper_bound(key);
	cr_expect(it == m.end());

	key = 4;
	it = m.lower_bound(key);
	cr_expect(it == m.end());
	it = m.upper_bound(key);
	cr_expect(it == m.end());

	key = -1;
	it = m.lower_bound(key);
	cr_expect(it->first == 0);
	it = m.upper_bound(key);
	cr_expect(it->first == 0);

}

Test(equal_range, test, .init = setup)
{
	cr_expect(((m.equal_range(0)).first)->first == 0);
	cr_expect(((m.equal_range(0)).second)->first == 1);

	cr_expect(((m.equal_range(-1)).first)->first == 0);
	cr_expect(((m.equal_range(-1)).second)->first == 0);

	cr_expect(((m.equal_range(4)).first) == m.end());
	cr_expect(((m.equal_range(4)).second) == m.end());
}

Test(swap, test, .init = setup)
{
	ft::map<int, std::string> m2;
	m2.insert(ft::make_pair(10, "ten"));
	m2.insert(ft::make_pair(15, "vijftien"));
	m2.insert(ft::make_pair(20, "twintig"));
	m2.insert(ft::make_pair(25, "vijfentwintig"));
	m2.insert(ft::make_pair(30, "dertig"));

	ft::map<int, std::string>::iterator it, it2;
	it = m.begin();
	it2 = m2.begin();

	m.swap(m2);

	cr_expect(it->first == 0 && it2->first == 10);
	cr_expect(m.size() == 5 && m2.size() == 4);
	cr_expect(m2[0] == "zero" && m[10] == "ten");

}

// TODO
// test get_allocator

// change < by _compare
// create a node_allocator object that uses std::allocator and has methods
// 	new_node and delete_node. Node kv_pair would become a pointer in this case