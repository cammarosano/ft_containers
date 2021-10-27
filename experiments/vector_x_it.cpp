#include <iostream>
#include <vector>

int main()
{
	std::vector<int> v;
	std::vector<int> v2;

	v.push_back(50);
	v.push_back(42);
	v.push_back(19);

	v2.assign(v.end(), v.begin());

	std::cout << "v2 size: " << v2.size() << std::endl;



}