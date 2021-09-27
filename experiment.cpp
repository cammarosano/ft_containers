#include <vector>
#include <iostream>

int main()
{
	std::vector<int> v1;
	v1.push_back(0);
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	std::cout << v1.capacity() << std::endl;
	std::vector<int> v2(v1);
	std::cout << v2.capacity() << std::endl;

}