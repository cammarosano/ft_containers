#include <criterion/criterion.h>
#include <iostream>
#include <map>
#include "ft_map.hpp"


std::map<int,std::string> std_map;
ft::map<int,std::string> m;
ft::map<int,std::string>::iterator it;
ft::map<int,std::string>::const_iterator cit;
ft::map<int,std::string>::reverse_iterator rit;
ft::map<int,std::string>::const_reverse_iterator crit;

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
	cr_expect(cit->first == 0);
	while (cit != m2.end())
	{
		// std::cout << cit->first << std::endl;
		cit++;
	}
	cr_expect((--cit)->first == 10);

}

Test(reverse_iterator, rbegin, .init=setup)
{
	rit = m.rbegin();
	cr_expect(rit->first == 10);
	cr_expect((--(m.rend()))->first == 0);
}

Test(const_reverse_iterator, test, .init=setup)
{
	ft::map<int, std::string> const m2(m);
	ft::map<int, std::string>::const_reverse_iterator crit = m2.rbegin();
	cr_expect(crit->first == 10);
	cr_expect((--(m2.rend()))->first == 0);
	// while (crit != m2.rend())
	// {
		// std::cout << crit->first << std::endl;
		// ++crit;
	// }
}

Test(max_size, test)
{
	cr_expect(m.max_size() <= std_map.max_size());
}

Test(insert, hint, .init=setup)
{
	it = m.begin();
	while (it->first != 8)
		++it;
	m.insert(it, ft::make_pair(9, "nine"));
	// m.print_tree();
}

Test(erase, position, .init=setup)
{
	it = m.begin();
	m.erase(++it);
}

Test(erase, range, .init=setup)
{
	m.erase(m.begin(), --(m.end()));
}

Test(erase, key, .init=setup)
{
	m.erase(6);
	// m.print_tree();
	m.erase(2);
}

Test(swap, test, .init=setup)
{
	ft::map<int,std::string> m2;

	m2[42] = "yo";
	m2[18] = "yey";

	m2.swap(m);
	cr_expect(m.size() == 2);
	cr_expect(m2.size() == 6);
}

Test(clear, test, .init=setup)
{
	m.clear();
	cr_expect(m.size() == 0);
}

Test(find, test, .init=setup)
{
	it = m.find(4);
	cr_expect(it->first == 4);
	it = m.find(3);
	cr_expect(it == m.end());
}

Test(find, const_obj, .init=setup)
{
	ft::map<int,std::string> const m2(m);

	cit = m2.find(8);
	cr_expect(cit->second == "eight");
}

Test(count, test, .init=setup)
{
	cr_expect(m.count(2) == 1);
	cr_expect(m.count(3) == 0);
}

Test(bounds, test, .init=setup)
{
	cr_expect(m.lower_bound(4)->first == 4);
	cr_expect(m.upper_bound(4)->first == 6);
	cr_expect(m.lower_bound(15) == m.end());
	cr_expect(m.upper_bound(15) == m.end());
	cr_expect(m.equal_range(2).first->first == 2);
	cr_expect(m.equal_range(2).second->first == 4);

	ft::map<int,std::string> const m2(m);

	cr_expect(m2.lower_bound(4)->first == 4);
	cr_expect(m2.upper_bound(4)->first == 6);
	cr_expect(m2.lower_bound(15) == m2.end());
	cr_expect(m2.upper_bound(15) == m2.end());
	cr_expect(m2.equal_range(2).first->first == 2);
	cr_expect(m2.equal_range(2).second->first == 4);

}

Test(get_allocator, test, .init=setup)
{
	ft::map<int, std::string>::allocator_type my_allocator = m.get_allocator();
	ft::map<int, std::string>::value_type * pair = my_allocator.allocate(2);
	my_allocator.construct(pair, ft::make_pair(42, "forty-two"));
	my_allocator.construct(pair + 1, ft::make_pair(19, "nineteen"));
	cr_expect(pair->first == 42);
	cr_expect((pair + 1)->second == "nineteen");
	my_allocator.destroy(pair);
	my_allocator.destroy(pair + 1);
	my_allocator.deallocate(pair, 2);
}

