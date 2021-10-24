#include <map>
#include <iostream>

int main()
{
	std::map<int, int> m;
	m.insert(std::make_pair(2, 4));
	m.insert(std::make_pair(3, 4));

	std::map<int,int>::iterator it = m.begin();
	std::pair<const int,int> * p = &(*it);
}