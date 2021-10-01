#include <map>
#include <utility>
#include <string>
#include <iostream>

int main()
{
	std::map<std::string, int> map;

	map["hello"] = 1;
	map["there"] = 2;

	std::cout << map["ther"] << std::endl;

	std::cout << map.size() << std::endl;

	std::map<std::string, int>::iterator it = map.begin();

	while (it != map.end())
	{
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}

	map.insert(std::pair<std::string, int>("new_key", 42));

	std::cout << map["new_key"] << std::endl;
	it = map.begin();

	while (it != map.end())
	{
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}

}