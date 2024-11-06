#include <iostream>
#include <algorithm> 

class Foo
{
private:
	int* array, size;
public:
	Foo(int s) : size(s), array(new int[s] {0})
	{
	}

	~Foo()
	{
		std::cout << "Destructor called, size: " << size << std::endl;
		delete[] array;
	}

	// Copy constructor
	Foo(const Foo& rhs) : size(rhs.size), array(new int[rhs.size])
	{
		std::cout << "Copy constructor called" << std::endl;
		std::copy(rhs.array, rhs.array + size, array);
	}

	// Move constructor for r-value
	Foo(Foo&& rhs) noexcept : size(rhs.size), array(rhs.array)
	{
		std::cout << "Move constructor called" << std::endl;
		rhs.size = 0;
		rhs.array = nullptr;
	}

	// Concatenation operator - r-value
	Foo operator+(const Foo& rhs)
	{
		std::cout << "Concatenation operator called" << std::endl;
		Foo result(size + rhs.size);
		std::copy(array, array + size, result.array);
		std::copy(rhs.array, rhs.array + rhs.size, result.array + size);

		return result;
	}

	// Copy assignment operator
    Foo& operator=(const Foo& rhs)
    {
        std::cout << "Copy assignment operator called" << std::endl;
        if (this == &rhs)
        {
            return *this;
        }

        delete[] array;
        size = rhs.size;
        array = new int[size];
        std::copy(rhs.array, rhs.array + size, array);

        return *this;
    }

	// Move assignment operator
	Foo& operator=(Foo&& rhs) noexcept
	{
		std::cout << "Move assignment operator called" << std::endl;
		if (this != &rhs)
		{
			delete[] array;
			size = rhs.size;
			array = rhs.array;
			rhs.size = 0;
			rhs.array = nullptr;
		}
		return *this;
	}

	// Overload array operator
	int& operator[](int index)
	{
		if (index < 0 || index >= size)
		{
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}

	int length() const
	{
		return size;
	}

	// Overload output stream operator
	friend std::ostream& operator<<(std::ostream& os, const Foo& rhs)
	{
		for (int i = 0; i < rhs.size; i++)
		{
			os << rhs.array[i] << " ";
		}
		return os;
	}
};

int main() {
	// Default construct
	std::cout << "Creating foo" << std::endl;
	Foo foo(3);
	for (int i = 0; i < foo.length(); i++)
	{
		foo[i] = i + 1;
	}
	std::cout << "foo: " << foo << std::endl;

	// Copy construct
	std::cout << "Creating goo from foo" << std::endl;
	Foo goo(foo);
	std::cout << "goo: " << goo << std::endl;

	// Move construct
	std::cout << "Creating woo" << std::endl;
	Foo woo(4);
	for (int i = 0; i < woo.length(); i++)
	{
		woo[i] = i * 2;
	}
	std::cout << "woo: " << woo << std::endl;

	std::cout << "Assigning hoo to foo + woo" << std::endl;
	// Create an r-value that's an unnamed memory location;
	// However, RVO/NRVO (Named/Return Value Optimization) will prevent the move constructor from being called since C++17
	// Also optional in C++14 as well
	auto hoo = foo + woo; 
	std::cout << "hoo (foo + woo): " << hoo << std::endl;

	Foo joo = std::move(hoo);
	return 0;
}