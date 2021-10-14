#include "ft_map.hpp"
#include <iostream>


int main()
{
	ft::map<int,int> m;

	m[0] = 0;
	m[1] = 0;
	m[-1] = 0;
	m[5] = 0;
	m[6] = 0;
	m[10] = 0;
	m[9] = 0;

	m.print();

	m.erase(0);
	m.print();

	


}