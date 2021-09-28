#include <criterion/criterion.h>
#include <iostream>
#include "ft_vector.hpp"
#include <vector>
#include <algorithm>
#include <string>

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
	ft::vector<int>::iterator::iterator(address) {}
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
	ft::swap(v, v2);
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
		std::cout << *rit++ << " ";
		++i;
	}
	cr_expect(i == v.size());
	std::cout << std::endl;
	
}
