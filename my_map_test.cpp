#include "ft_map.hpp"
#include "utils.hpp"
#include <map>

class my_comp
{
public:
	bool operator()(int const &a, int const &b)
	{
		return (a < b);
	}
};

int main()
{
	std::map<int, int, my_comp> m;

	m[0] = 1;
	m[2] = 2;

	std::map<int,int, my_comp> const m2(m); // won't compile 'cause 
	// my_comp become const my_comp, and operator() is not marked as const member function

	std::map<int,int,my_comp>::const_iterator cit = m2.find(2);

	std::cout << cit->first << std::endl;


}