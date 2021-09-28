#include <vector>
#include <iostream>

struct experiment
{
	int a;
	int b;
};


int main()
{
	std::vector<experiment> v1;
	v1.push_back(experiment());
	v1.push_back(experiment());

	std::vector<experiment>::iterator it = v1.begin();
	(*it).a = 5;
	it->b = 6;
	std::cout << it->a << std::endl;
	std::cout << (*it).b << std::endl;

}