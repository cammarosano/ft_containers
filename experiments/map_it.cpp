#include <map>
#include <iostream>

int main()
{
	std::map<int,int>::iterator it;

	std::cout << &*it << std::endl;
	std::cout << &it << std::endl;
	
}