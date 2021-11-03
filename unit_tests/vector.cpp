#include <criterion/criterion.h>
#include <iostream>
#include "vector.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <csignal>
#include <set>
#include "utils.hpp"


// overload for debug and test
template<typename T>
std::ostream & operator<<(std::ostream & out, ft::vector<T> const & v)
{
	for (size_t i = 0; i < v.size(); i++)
		out << v[i] << " ";
	out << std::endl;
	return (out);
}

ft::vector<int> v;
std::vector<int> stl_vector;

void setup(void)
{
	for (size_t i = 0; i < 20; i++)
	{
		v.push_back(i);
		stl_vector.push_back(i);
	}
}

Test(misc, string)
{
	ft::vector<std::string> vs;
	vs.push_back("hello");
	vs.push_back("this is");
	vs.push_back("a test!");

	std::sort(vs.begin(), vs.end());
	for (size_t i = 0; i < vs.size(); i++)
	{
		// std::cout << vs[i] << ", ";
		if (i)
			cr_assert(vs[i - 1] < vs[i]);
	}
	// std::cout << std::endl;
}

Test(constructor, default_constructor)
{
	ft::vector<int> v;

	cr_assert(v.size() == 0, "size should be 0");
	cr_assert(v.capacity() == 0, "capacity should be 0");
}

Test(constructor, n_constructor)
{
	ft::vector<int> v(5, 42);

	cr_expect(v.size() == 5, "size should be 5");
	for (size_t i = 0; i < v.size(); i++)
		cr_expect(v[i] == 42, "v[i] should be 42");
}

Test(constructor, copy_constructor, .init=setup)
{
	ft::vector<int> v2(v);
	cr_assert(v.size());
	cr_expect(v2.size() == v.size());
	cr_expect(v2.front() == v.front());
	cr_expect(v2.back() == v.back());

	ft::vector<int> v3;	// empty
	ft::vector<int> v4(v3);
	cr_expect(v4.size() == 0);
}

Test(constructor, range_constructor, .init=setup)
{
	ft::vector<int> v2(stl_vector.begin(), stl_vector.end());
	cr_assert(stl_vector.size());
	cr_expect(v2.size() == stl_vector.size());
	cr_expect(v2.front() == stl_vector.front());
	cr_expect(v2.back() == stl_vector.back());
}

Test(modify, push_back)
{
	ft::vector<int> my_vector;
	std::vector<int> stl_vector;

	for (size_t i = 0; i < 16; i++)
	{
		my_vector.push_back(i);
		stl_vector.push_back(i);
		cr_expect(my_vector.size() == stl_vector.size());
		cr_expect(my_vector.capacity() == stl_vector.capacity());
		cr_expect(my_vector.back() == stl_vector.back());
	}
}

Test(iterator, algorithm_compability, .init=setup)
{
	ft::vector<int> v;

	size_t n = 20;
	for (size_t i = 0; i < n; i++)
		v.push_back(n - i);
	std::sort(v.begin(), v.end());
	for (size_t i = 1; i < n; i++)
		cr_expect(v[i - 1] <= v[i]);
	
}

Test(element_access, at, .init = setup)
{
	try
	{
		v.at(v.size());
	}
	catch(const std::exception& e)
	{
		cr_expect(true, "this exception should be raised");
	}
	try
	{
		v.at(v.size() - 1);
		cr_expect(true);
	}
	catch(const std::exception& e)
	{
		cr_expect(false, "this should not raise an exception");
	}
	
}

Test(modify, clear, .init = setup)
{
	cr_assert(v.size());
	v.clear();
	cr_expect(v.size() == 0);
}

Test(modify, assign_n, .init=setup)
{
	v.assign(5, 42);
	cr_expect(v.size() == 5);
	for (size_t i = 0; i < v.size(); i++)
		cr_expect(v[i] = 42);
	
	v.assign(100, 19);
	cr_expect(v.size() == 100);
	for (size_t i = 0; i < v.size(); i++)
		cr_expect(v[i] = 19);
}

Test(modify, assign_range, .init=setup)
{
	std::vector<int> stl_v;
	stl_v.push_back(66);
	stl_v.push_back(64);
	stl_v.push_back(62);
	stl_v.push_back(60);
	stl_v.push_back(162);
	stl_v.push_back(262);
	stl_v.push_back(362);

	v.assign(stl_v.begin(), stl_v.end());
	for (size_t i = 0; i < v.size(); i++)
		cr_expect(v[i] == stl_v[i]);
	stl_v.clear();
	v.assign(stl_v.begin(), stl_v.end());
	cr_expect(v.size() == 0);
}

class InputIteratorTest: public ft::vector<int>::iterator
{
public:
	typedef std::input_iterator_tag iterator_category;

	InputIteratorTest(int * address):
	ft::vector<int>::iterator::vector_iterator(address) {}
};

Test(modify, assign_range_input_it, .init=setup)
{
	int *ptr = &(*(v.begin()));
	InputIteratorTest IIbeg(ptr);
	InputIteratorTest IIend(ptr + v.size());
	ft::vector<int> new_v;
	new_v.assign(IIbeg, IIend);	
	// std::cout << new_v.size() << std::endl;
	for (size_t i = 0; i < new_v.size(); i++)
		cr_expect(v[i] == new_v[i]);
}

Test(modify, insert, .init=setup)
{
	ft::vector<int>::iterator it = v.insert(v.begin() + 1, 42);
	cr_expect(*it == 42);
	// std::cout << v;

	size_t old_size = v.size();
	int n = 30;
	v.insert(v.begin() + 3, n, 19);
	cr_expect(v.size() == old_size + n);
	it = v.begin() + 3;
	while (n--)
		cr_expect(*it++ == 19);
	// std::cout << v;

	n = 10;
	old_size = v.size();
	v.insert(v.end(), stl_vector.end() - n, stl_vector.end());
	cr_expect(v.size() == old_size + n);
	// std::cout << v;
	while (n--)
	{
		cr_expect(v.back() == stl_vector.back());
		v.pop_back();
		stl_vector.pop_back();
	}

	// insert in an empty vector
	ft::vector<int> new_vector;
	new_vector.insert(new_vector.begin(), 5, 42);
	cr_expect(new_vector.size() == 5);

	ft::vector<int> v2;
	v2.insert(v2.end(), v.begin(), v.begin() + 5);
	cr_expect(v2.size() == 5);
}

Test(modify, swap_member, .init=setup)
{
	ft::vector<int> v2(5, 42);

	size_t v_old_size = v.size();
	v.swap(v2);
	cr_expect(v.size() == 5);
	cr_expect(v.front() == 42);
	cr_expect(v.back() == 42);
	cr_expect(v2.size() == v_old_size);

	// std::cout << "v: " << v;
	// std::cout << "v2: " << v2;
}

Test(modify, swap_non_member, .init=setup)
{
	ft::vector<int> v2(5, 42);

	size_t v_old_size = v.size();
	// ft::swap(v, v2);
	swap(v, v2);
	cr_expect(v.size() == 5);
	cr_expect(v.front() == 42);
	cr_expect(v.back() == 42);
	cr_expect(v2.size() == v_old_size);

	// std::cout << "v: " << v;
	// std::cout << "v2: " << v2;

}

Test(allocator, get_allocator, .init=setup)
{
	int *p = v.get_allocator().allocate(5);

	for (int i = 0; i < 5; i++)
	{
		v.get_allocator().construct(p + i, 100 + i);
	}
	for (int i = 0; i < 5; i++)
	{
		cr_expect(p[i] == 100 + i);
		// std::cout << p[i] << " ";
	}
	// std::cout << std::endl;
	v.get_allocator().deallocate(p, 5);
}

Test(iterator, reverse_iterator, .init= setup)
{
	ft::vector<int>::reverse_iterator rit, rite;

	rit = v.rbegin();
	rite = v.rend();

	cr_expect(*rit == v.back());
	cr_expect(*(--rite)++ == v.front());
	size_t i = 0;
	while (rit != rite)
	{
		// std::cout << *rit << " ";
		++rit;
		++i;
	}
	// std::cout << std::endl;
	cr_expect(i == v.size());
	
	rit = v.rbegin();
	cr_expect(rit + v.size() == v.rend());
	while (rit < rite)
	{
		// std::cout << *rit << " ";
		rit = rit + 2;
	}
	// std::cout << std::endl;
}

Test(iterator, overloads, .init=setup)
{
	ft::vector<int>::iterator it;
	it = v.begin();

	cr_expect(*(3 + it) == v[3]);
	cr_expect((it + 3) - it == 3);
}

Test(iterator, reverse_it_overloads, .init=setup)
{
	ft::vector<int>::reverse_iterator it, it2;

	it = v.rbegin();
	it2 = it;

	++it;
	++it;
	cr_expect(it - it2 == 2);

	it = v.rend();
	it -= 4;
	cr_assert(*it == v[3]);
	it += 2;
	cr_assert(*it == v[1]);
}

// Test(iterator, stl_vector, .signal = SIGSEGV )
// {
// 	std::vector<int> sv;

// 	std::vector<int>::reverse_iterator rit = sv.rend();
// 	--rit;
	// std::cout << *rit << std::endl;  // crashes (-fsanitize=address must be disabled)
// }

Test(reverse_iterator, operator_brackets, .init=setup)
{
	ft::vector<int>::reverse_iterator rit = v.rbegin();

	// std::cout << v;
	// std::cout << rit[0] << std::endl;
	// std::cout << rit[1] << std::endl;
	cr_expect(rit[0] == v[v.size() - 1]);
	cr_expect(rit[1] == v[v.size() - 2]);

}

struct hello
{
	int a;
	int b;
};

Test(iterator, arrow_operator, .init=setup)
{
	ft::vector<hello> vec;
	vec.push_back(hello());
	vec.push_back(hello());
	ft::vector<hello>::iterator it = vec.begin();
	it->a = 5;
	cr_expect(it->a == 5);

	ft::vector<hello>::reverse_iterator rit = vec.rbegin();
	rit++;
	rit->b = 42;
	cr_expect(it->b == 42);

}

Test(iterator, const_iterator)
{
	ft::vector<int> const v(5, 42);

	ft::vector<int>::const_iterator cit = v.begin();

	while (cit != v.end())
	{
		// std::cout << *cit << " ";
		cr_expect(*cit == 42);
		++cit;
	}
	// std::cout << std::endl;

	cit = v.begin();
	// *cit = 43; // does not compile ( = GOOD!)
}

Test(iterator, const_reverse_iterator, .init=setup)
{
	ft::vector<int> const vc(v);

	ft::vector<int>::const_reverse_iterator crit = vc.rbegin();
	cr_expect(*crit == vc.back());
	while (crit != vc.rend())
	{
		// std::cout << *crit << " ";
		crit++;
	}
	// std::cout << std::endl;
	cr_expect(*(--crit) == vc.front());
	crit = vc.rbegin();
	// *crit = 42; // does not compilte ( = GOOD!)
}

Test(iterator, const_it_construction, .init=setup)
{
	ft::vector<int>::const_iterator cit(v.begin());

	cr_expect(*cit == v.front());

	// ft::vector<int>::iterator normal_it(cit); // not possible: GOOD!

	ft::vector<int>::const_reverse_iterator crit(v.rbegin());
	cr_expect(*crit == v.back());

	// ft::vector<int>::reverse_iterator rit(crit); // does not compile (GOOD!)

}

Test(relational_operators, equal_less)
{
	ft::vector<int> v1(5, 42);
	ft::vector<int> v2(6, 42);

	cr_expect(!(v1 == v2));
	cr_expect(v1 < v2);

	v2.pop_back();
	cr_expect(v1 == v2);
	cr_expect(!(v1 < v2));

	ft::vector<std::string> vs1;
	ft::vector<std::string> vs2;

	vs1.push_back("hello");
	vs1.push_back("there!");

	vs2.push_back("hello");
	vs2.push_back("there");

	cr_expect(!(vs1 == vs2));
	cr_expect(vs2 < vs1);

	vs2.back().append(1, '!');
	cr_expect(vs1 == vs2);
	cr_expect(!(vs1 < vs2));
}

Test(relational_operators, others)
{
	ft::vector<int> v1(2, 200);
	ft::vector<int> v2(3, 100);

	cr_expect(v1 != v2);
	cr_expect(!(v1 == v2));
	cr_expect(v1 > v2);
	cr_expect(v1 >= v2);
	cr_expect(v2 < v1);
	cr_expect(v2 <= v1);
}

Test(distance, test)
{
	ft::vector<int> v;
	// std::vector<int> v;
	std::set<int> s;
	s.insert(1);
	s.insert(3);
	s.insert(5);
	s.insert(-1);
	v.assign(s.begin(), s.end());
}

Test(assignation_op, test, .init=setup)
{
	ft::vector<int> v2;

	v2 = v;

	cr_expect(v2.size() == v.size());
	for (size_t i = 0; i < v.size(); i++)
		cr_expect(v2[i] == v[i]);
}

Test(assign, exception, .init=setup)
{
	ft::vector<int> v2;

	try
	{
		v2.assign(v.end(), v.begin());
		cr_expect(0);
	}
	catch (std::exception const &)
	{
		cr_expect(1);
	}
}

Test(erase, range, .init=setup)
{
	v.erase(v.begin(), v.begin() + 5);
	cr_expect(v[0] == 5);
	v.erase(v.begin() + 1, v.begin() + 5);
	cr_expect(v[1] == 10);
	// std::cout << v << std::endl;
}

Test(erase, position, .init=setup)
{
	v.erase(v.begin() + 10);
	cr_expect(v[10] = 11);
	v.erase(v.end() - 1);
	cr_expect(v.size() == 18);
	// std::cout << v << std::endl;
}

Test(bool, test)
{
	ft::vector<bool> v;

	v.push_back(true);
	v.push_back(false);
	v.push_back(true);
	v.push_back(false);

	cr_expect(v.size() == 4);
	cr_expect(v.front() == true);
	cr_expect(v.back() == false);
}

bool pred_func(int a, int b)
{
	if (a % 2)
		return (b % 2);
	return (!(b % 2));
}

class Predicate
{
public:
	bool operator()(int a, int b)
	{
		if (a % 2)
			return (b % 2);
		return (!(b % 2));
	}
};

Test(equal, predicate, .init = setup)
{
	ft::vector<int> v2(v);
	cr_expect(ft::equal(v.begin(), v.end(), v2.begin(), pred_func));
	cr_expect(ft::equal(v.begin(), v.end(), v2.begin(), Predicate()));
}

Test(lexic_comp, custom, .init = setup)
{
	ft::vector<int> v2(v);
	cr_expect(!ft::lexicographical_compare(v.begin(), v.end(),
				v2.begin(), v2.end(), pred_func));
	cr_expect(!ft::lexicographical_compare(v.begin(), v.end(),
				v2.begin(), v2.end(), Predicate()));
}

enum e_test
{
	e_1, e_2, e_3
};

Test(is_integral, const_volatile)
{
	cr_expect(ft::is_integral<bool>::value);
	cr_expect(ft::is_integral<char>::value);
	cr_expect(ft::is_integral<signed char>::value);
	cr_expect(ft::is_integral<wchar_t>::value);
	cr_expect(ft::is_integral<short>::value);
	cr_expect(ft::is_integral<int>::value);

	cr_expect(!ft::is_integral<float>::value);
	cr_expect(!ft::is_integral<double>::value);
	cr_expect(!ft::is_integral<double>::value);
	cr_expect(!ft::is_integral<e_test>::value);
}

Test(iterator, comp_const_it, .init=setup)
{
	ft::vector<int>::iterator it = v.begin();
	ft::vector<int>::const_iterator cit = v.begin();

	cr_expect(it == cit);
	cr_expect(*it == *cit);

	ft::vector<int>::const_iterator cite = v.end();
	while (it < cite && it != cite)
	{
		cr_expect(it == cit);
		it++;
		++cit;
	}
}
