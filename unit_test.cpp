#include <criterion/criterion.h>
#include "ft_vector.hpp"
#include <vector>
#include <algorithm>


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

Test(iterator, algorithm_compability)
{
	ft::vector<int> v;

	size_t n = 20;
	for (size_t i = 0; i < n; i++)
		v.push_back(n - i);
	std::sort(v.begin(), v.end());
	for (size_t i = 1; i < n; i++)
		cr_expect(v[i - 1] <= v[i]);

}

Test(element_access, at)
{
	ft::vector<int> v(5, 42);
	try
	{
		v.at(5);
	}
	catch(const std::exception& e)
	{
		cr_expect(true, "this exception should be raised");
	}
	try
	{
		v.at(4);
		cr_expect(true);
	}
	catch(const std::exception& e)
	{
		cr_expect(false, "this should not raise an exception");
	}
	
}