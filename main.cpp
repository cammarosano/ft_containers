#include "ft_vector.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	ft::vector<int> v;

	std::cout << "size: " << v.size() << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;

	int n = 16;
	for (int i = 0; i < n; i++)
	{
		v.push_back(n - i);
		std::cout << "size: " << v.size() << std::endl;
		std::cout << "capacity: " << v.capacity() << std::endl;
	}

	std::cout << "vector content: " << std::endl;
	ft::vector<int>::iterator it = v.begin();
	ft::vector<int>::iterator ite = v.end();
	while (it != ite)
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;

	// testing if my iterators work with algo lib
	std::sort(v.begin(), v.end());

	std::cout << "vector content after sorting: " << std::endl;
	it = v.begin();
	ite = v.end();
	while (it != ite)
	{
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;

	std::cout << v[3] << std::endl;

	try
	{
		std::cout << v.at(23) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << "front(): " << v.front() << std::endl;
	std::cout << "back(): " << v.back() << std::endl;

	// ft::vector<int> vec2;
	// std::cout << "back(): " << vec2.back() << std::endl;

	// max_size
	std::vector<int> vec2;
	std::cout << vec2.get_allocator().max_size() << std::endl;
	std::cout << vec2.max_size() << std::endl;

	std::cout << v.max_size() << std::endl;

	// resize()
	v.resize(2);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	v.resize(20, 66);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	v.resize(20, 66);
	
	// empty()
	std::cout << v.empty() << std::endl;
	v.resize(0);
	std::cout << v.empty() << std::endl;

	// reserve();
	v.reserve(99);
	std::cout << v.capacity() << std::endl;

	// pop_back():
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	while (!v.empty())
	{
		std::cout << v.back() << std::endl;
		v.pop_back();
	}
	std::cout << v.capacity() << std::endl;

	// assign(n, val)
	// v.assign(static_cast<size_t>(0), 15);
	v.assign(0, 15);
	std::cout << v.capacity() << std::endl;
	std::cout << v.empty() << std::endl;
	std::cout << v.size() << std::endl;
	// v.assign(static_cast<size_t>(42), 42);
	v.assign(42, 42);
	std::cout << v.capacity() << std::endl;
	std::cout << v.empty() << std::endl;
	std::cout << v.size() << std::endl;
	v.assign(static_cast<size_t>(42), -1);
	std::cout << v.capacity() << std::endl;
	std::cout << v.empty() << std::endl;
	std::cout << v.size() << std::endl;

	// assign(range)
	{
		std::vector<int> std_vector;
		std_vector.push_back(2);
		std_vector.push_back(4);
		std_vector.push_back(8);
		std_vector.push_back(16);
		std::vector<int>::iterator it, ite;
		it = std_vector.begin();
		ite = std_vector.end();
		v.assign(it, ite);
		for (size_t i = 0; i < v.size(); i++)
			std::cout << v[i] << " ";
		std::cout << std::endl;
	}

	// clear
	v.clear();
	std::cout << "size: " << v.size() << std::endl;

	// erase(position)
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	it = v.begin() + 3;
	ft::vector<int>::iterator itx = v.erase(it);
	std::cout << *itx << std::endl;
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	// v.erase(it); // segfaults

	{	// STL
		std::vector<int> vec;
		vec.push_back(0);
		vec.push_back(1);
		vec.push_back(2);
		vec.push_back(3);
		for (size_t i = 0; i < vec.size(); i++)
			std::cout << vec[i] << " ";
		std::cout << std::endl;

		std::vector<int>::iterator it = vec.begin() + 3;
		for (size_t i = 0; i < vec.size(); i++)
			std::cout << vec[i] << " ";
		std::cout << std::endl;
		
		(void)it;
		// vec.erase(it); // segfaults
	}

	/* erase(range) */

	v.push_back(10);
	v.push_back(11);
	v.push_back(12);
	v.push_back(13);
	v.push_back(14);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	ft::vector<int>::iterator first, last;
	first = v.begin() + 3;
	last = first + 3;  // 10 11 12 should disappear
	it = v.erase(first, last);
	std::cout << *it << std::endl; // should be 13
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	/* insert(position, val) */

	v.insert(v.end(), 66);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;
	v.assign(99, 42);
	std::cout << "capacity: " << v.capacity() << std::endl;
	v.pop_back();
	it = v.insert(v.end() - 2, 66);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	std::cout << *it << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;

	/* insert(position, n , val) */
	v.insert(v.begin() + 2, 5, -1);
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << " ";
	std::cout << std::endl;

	/* insert(position, first , last) */
	{
		std::vector<int> std_vec;
		std_vec.push_back(0);
		std_vec.push_back(2);
		std_vec.push_back(4);
		std_vec.push_back(6);

		ft::vector<int> ve;
		ve.push_back(1);
		ve.push_back(3);
		ve.push_back(5);
		
		std::cout << "size: " << ve.size() << "| capacity: " << ve.capacity() << std::endl;
		ve.insert(ve.begin(), std_vec.begin(), std_vec.end());
		for (size_t i = 0; i < ve.size(); i++)
			std::cout << ve[i] << " ";
		std::cout << std::endl;

	}
}
