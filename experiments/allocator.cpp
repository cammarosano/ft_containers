#include <memory>
#include <iostream>

class Yo
{
private:
	int val;
public:
	Yo() { std::cout << "default constructor\n";}
	Yo(int val): val(val)
	{ std::cout << "parameter constructor\n";}
	Yo( Yo const & src): val(src.val)
	{std::cout << "copy constructor\n";}
	~Yo() {std::cout << "destructor\n";}

	int getVal(void) const
	{ return (val);}
};


int main()
{
	std::allocator<Yo> allocator;

	Yo * ptr = allocator.allocate(5);
	std::cout << "allocation done\n";
	for (size_t i = 0; i < 5; i++)
	{
		allocator.construct(&ptr[i], Yo(i));
	}

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << ptr[i].getVal() << " ";
	}
	std::cout << std::endl;

	Yo * ptr2 = allocator.allocate(5, ptr);

	std::cout << ptr << std::endl;
	std::cout << ptr2 << std::endl;

	for (size_t i = 0; i < 5; i++)
	{
		allocator.destroy(&ptr[i]);
	}
	allocator.deallocate(ptr, 5);

	
	
}