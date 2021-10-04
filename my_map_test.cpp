#include <string>
#include "ft_map.hpp"
#include <iostream>

int main()
{
	ft::map<int, std::string> m;

	m[42] = "forty-two";

	std::cout << m[42] << std::endl;
	std::cout << m[41] << std::endl;




}