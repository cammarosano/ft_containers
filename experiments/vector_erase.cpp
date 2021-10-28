#include <vector>
#include <iostream>

int main()
{
	std::vector<int> v(5, 42);
	std::cout << v.size() << std::endl;

	v.erase(v.end());


}