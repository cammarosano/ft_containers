#include <criterion/criterion.h>
#include <iostream>
#include "ft_map.hpp"

ft::map<int,std::string> m;

Test(insert, key)
{

	cr_expect(m.insert(ft::make_pair(19, "yo")).second);
	cr_expect(m.insert(ft::make_pair(21, "yo")).second);
	cr_expect(!m.insert(ft::make_pair(21, "yo")).second);
	cr_expect(m.insert(ft::make_pair(15, "yo")).first->first == 15);

}