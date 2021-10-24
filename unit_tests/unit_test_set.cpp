#include <criterion/criterion.h>
#include <iostream>
#include "ft_set.hpp"
#include <set>
#include <vector>

ft::set<int> s;
ft::set<int>::iterator it;
ft::set<int>::const_iterator cit;
ft::set<int>::reverse_iterator rit;
ft::set<int>::const_reverse_iterator crit;
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

Test(erase, position, .init=setup)
{
	it = s.begin();
	s.erase(it);
	cr_expect(*s.begin() == 10);
}

Test(erase, key, .init=setup)
{
	cr_expect(s.erase(15) == 1);
	cr_expect(s.size() == 4);
	cr_expect(s.erase(15) == 0);
	cr_expect(s.size() == 4);
}

Test(erase, range, .init=setup)
{
	s.erase(++s.begin(), --s.end());
	cr_expect(s.size() == 2);
	s.erase(s.begin(), s.end());
	cr_expect(s.size() == 0);

}

Test(swap, test, .init=setup)
{
	ft::set<int> s2;
	s2.insert(66);
	s2.insert(69);
	s2.insert(666);
	s2.insert(999);

	s.swap(s2);
	cr_expect(s.size() == 4);
	cr_expect(s2.size() == 5);

	swap(s, s2);
	cr_expect(s2.size() == 4);
	cr_expect(s.size() == 5);
}

Test(clear, test, .init=setup)
{
	s.clear();
	cr_expect(s.size() == 0);
	cr_expect(s.begin() == s.end());
}

bool test_comp(int a, int b)
{
	return (a < b);
}

Test(key_comp, value_comp, .init=setup)
{
	ft::set<int, bool (*)(int, int)> s(test_comp);
	ft::set<int, bool (*)(int, int)>::key_compare kc = s.key_comp();
	ft::set<int, bool (*)(int, int)>::value_compare vc = s.value_comp();

	cr_expect(kc(1, 2));
	cr_expect(vc(2, 3));
	cr_expect(!kc(3, 2));
	cr_expect(!vc(3, 3));
}

Test(constructor, copy, .init=setup)
{
	ft::set<int> s2(s);
	it = s2.begin();
	cr_expect(*it == *(s.begin()));
	cr_expect(s2.size() == s.size());
}

Test(const_iterator, test, .init=setup)
{
	ft::set<int> const s2(s);

	cit = s2.begin();
	cr_expect(*cit == 5);
	while (cit != s2.end())
		++cit;
	--cit;
	cr_expect(*cit == 25);
}

Test(constructor, range, .init=setup)
{
	std::vector<int> v;
	v.push_back(10);
	v.push_back(11);
	v.push_back(12);
	v.push_back(13);

	ft::set<int> const s(v.begin(), v.end());
	cr_expect(*(s.begin()) == 10);
	cr_expect(s.size() == 4);
}

Test(reverse_iterators, test, .init=setup)
{
	ft::set<int> const s2(s);
	rit = s.rbegin();
	crit = s2.rbegin();
	while (crit != s2.rend())
	{
		cr_expect(*crit == *rit);
		++crit;
		++rit;
	}
}

Test(find, count, .init=setup)
{
	it = s.find(20);
	cr_expect(*it == 20);
	cr_expect(*++it == 25);

	cr_expect(s.count(5) == 1);
	cr_expect(s.count(16) == 0);
}

Test(bounds, test, .init=setup)
{
	ft::set<int> const s2(s);

	cr_expect(*(s2.lower_bound(10)) == 10);
	cr_expect(*(s2.upper_bound(10)) == 15);
	cr_expect(*(s2.lower_bound(25)) == 25);
	cr_expect((s2.upper_bound(25)) == s2.end());

	cr_expect(*(s2.equal_range(20).first) == 20);
	cr_expect(*(s2.equal_range(20).second) == 25);
}

Test(relational_operators, test, .init=setup)
{
	ft::set<int> const s2(s);

	cr_expect(s == s2);
	cr_expect(!(s != s2));

	s.insert(30);
	cr_expect(!(s == s2));
	cr_expect(s != s2);
	cr_expect(s2 < s);
	cr_expect(!(s < s2));

	// todo : test > <= >=
	cr_expect(s > s2);
	cr_expect(s >= s2);
	s.erase(30);
	cr_expect(s >= s2);
	cr_expect(s <= s2);
	cr_expect(!(s < s2));
	s.erase(5);
	cr_expect(!(s2 > s)); // 1st elem of s is 10, s2's is 5
}
