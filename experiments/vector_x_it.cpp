#include <iostream>
#include <vector>
#include <iterator>
#include <map>

int main()
{
	std::vector<int> v;
	std::vector<int> v2;
	std::map<int,int> m;

	v.push_back(50);
	v.push_back(42);
	v.push_back(19);

	m.insert(std::make_pair(0,1));
	m.insert(std::make_pair(1,1));
	m.insert(std::make_pair(2,1));

	// try
	// {
		// v2.assign(v.end(), v.begin());
		v2.insert(v2.begin(), v.end(), v.begin());
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	// std::cout << std::distance(v.end(), v.begin()) << std::endl;
	// std::cout << std::distance(m.end(), m.begin()) << std::endl;


	std::cout << "v2 size: " << v2.size() << std::endl;

	// std::cout << v.max_size() << std::endl;

	// v.reserve(4611686018427387904);
	// v2.at(10);

	std::vector<int>::reverse_iterator rit = v.rend();


}