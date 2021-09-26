#include <iostream>

int main()
{
	int a[2] = {0, 1};
	int *p = a;

	int *p2 = 1 + p;
	std::cout << *p2 << std::endl;
}