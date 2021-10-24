#include <vector>
#include <iostream>

int main()
{
	std::vector<int> v;
	for (size_t i = 0; i < 10; i++)
		v.push_back(i);
	std::cout << v.capacity() << std::endl;

	std::vector<int> v2(v);
	std::cout << v2.capacity() << std::endl;
	
	std::vector<int> v3;
	v3 = v;
	std::cout << v3.capacity() << std::endl;
}