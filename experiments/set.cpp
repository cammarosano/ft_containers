#include <set>
#include <map>
#include <iostream>
#include <utility>

int main()
{
	std::set<int> s;

	s.insert(5);
	s.insert(10);
	s.insert(0);

	std::set<int>::iterator it = s.begin();

	std::cout << *it << std::endl;

	std::map<int, int> m;
	m[5] = 0;
	m[15] = 0;
	std::map<int,int>::iterator mit = m.begin();
	// *mit = std::pair<const int, int>(42, 42);


}