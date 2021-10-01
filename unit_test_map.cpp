#include <criterion/criterion.h>
#include <iostream>
#include "ft_map.hpp"
#include <string>

Test(constructor, default_construct)
{
	ft::map<int, std::string> m;
	cr_expect(m.size() == 0);
}

Test(pair, constructor)
{
	ft::pair<int,std::string> pr;

	pr.first = 1;
	pr.second = "hello";

	// std::cout << pr.first << " - " << pr.second << std::endl;
	cr_expect(pr.first == 1 && pr.second == "hello");

	ft::pair<int,std::string> pr2(2, "again");
	// std::cout << pr2.first << " - " << pr2.second << std::endl;
	cr_expect(pr2.first == 2 && pr2.second == "again");

	ft::pair<int,std::string> pr3(pr);
	cr_expect(pr3.first == 1 && pr3.second == "hello");

}

Test(insert, insert_pair)
{
	ft::map<int, std::string> m;

	ft::pair<int const, std::string> 	pr(42, "forty-two");
	bool ret = m.insert(pr);
	cr_expect(ret);


}