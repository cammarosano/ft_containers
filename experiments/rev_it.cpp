#include <map>
#include <map.hpp>

int main()
{
	std::map<int,int> m;

	std::map<int,int>::reverse_iterator rit, rit2;

	rit = m.rbegin();
	rit2 = m.rend();

	// int n =	rit2 - rit;
	
	// rit += 4; // compiler: no viable overloaded -=

	ft::map<int,int> ft_m;
	ft::map<int,int>::reverse_iterator ft_rit, ft_rit2;
	ft_rit = ft_m.rbegin();
	ft_rit2 = ft_m.rend();
	// ft_rit+= 4; // same compiler error :)

	int n = ft_rit - ft_rit2;


	

}