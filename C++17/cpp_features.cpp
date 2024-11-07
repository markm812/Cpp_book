#include <memory>
#include <string_view>
#include <array>
#include <iostream>


// Guaranteed copy elision
auto factory() {
	return std::make_unique<int>();
}

// fold expressions #2
template <typename... T>
auto add(const T & ... args) {
	return (args + ...);
}

int main() {
	// unique pointer are not supposed be to copied
	// no calling of std::move as well, 
	// and object is still being created in place
	auto p = factory();

	// constexpr lambdas
	constexpr auto l = []() { return 42; };

	// string_view with constexpr when you only want a view of the string
	constexpr std::string_view sv = "string_view";

	// class template argument deduction
	std::pair p1(1, 2.0); // std::pair<int, double>
	std::array data{ 1, 2, 3, 4 }; // std::array<int, 4>

	// fold expressions #1
	auto sum = [](auto... args) { return (... + args); };

	// structured bindings
	std::pair values{ 1,2 };
	auto [first, second] = values;

	// if-init statement
	if (auto result = add(1, 2); result > 2) {
		std::cout << "result is greater than 2\n";
	}

	return sum(1, 2, 3, 4);
}