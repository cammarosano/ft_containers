#include <vector>
#include <iostream>

int main()
{
	std::vector<int> vector;

	// std::cout << vector[0] << std::endl;

	int i = 0;
	int	n = 10;
	while (i < n)
	{
		vector.push_back(i);
		std::cout << "size: " << vector.size() 
				<< ", capacity: " << vector.capacity() << std::endl;
		i++;
	}
	std::vector<int>::iterator it = vector.begin();
	i = -1;
	while (++i < 5)
		std::cout << "it[" << i << "] : " << it[i] << std::endl;

	vector.push_back(42);
	vector.push_back(42);
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	std::cout << "Old iterator: " << std::endl;
	i = -1;
	while (++i < 5)
		std::cout << "it[" << i << "] : " << it[i] << std::endl;
	
	it = vector.begin();
	std::cout << "testing it[3]: " << it[3] << std::endl;
	std::cout << "New iterator: " << std::endl;
	i = -1;
	while (++i < 5)
		std::cout << "it[" << i << "] : " << it[i] << std::endl;

	// std::cout << (n += 5) << std::endl;	 just a test about +=
	int a = 5;
	int *p = &a;
	int *p1 = p + 1;

	std::cout << p1 - p << std::endl;
	std::vector<int>::reverse_iterator rit = vector.rbegin();
	std::vector<int>::reverse_iterator rite = vector.rend();
	while (rit < rite)
		std::cout << *rit++ << " ";
	std::cout << std::endl;

	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	vector.resize(1, 66);
	for (size_t i = 0; i < vector.size(); i++)
		std::cout << vector[i] << " ";
	std::cout << std::endl;
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	vector.push_back(0);
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	vector.reserve(17);
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	vector.assign(17, 42);
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;
	vector.insert(vector.begin(), 300);
	std::cout << "size: " << vector.size() 
			<< ", capacity: " << vector.capacity() << std::endl;

}