#include <iostream>
#include <algorithm>
#include <vector>

inline long c(const int superscript, const int subscript) {
	long upper = 1, lower = 1;
	long start = superscript;

	while (start) {
		upper *= subscript - (superscript - start);
		lower *= start;
		start--;
	}
	return upper / lower;
}

auto eq = [](std::vector<int>& a) {return std::all_of(a.begin(), a.end(), [&a](int n) {return n == a[0]; }); };
auto and = [](std::vector<int>& a) {return std::all_of(a.begin(), a.end(), [&a](int n) {return n && a[0]; }); };
auto or = [](std::vector<int>& a) {return std::any_of(a.begin(), a.end(), [&a](int n) {return n || a[0]; }); };
auto max = [](std::vector<int>& a) {return *max_element(a.begin(), a.end()); };
int main() {
	std::vector<int> a = { 0,0,0,1,3 };
	std::cout << max(a) << std::endl;
	//std::cout << c(4, 5) << std::endl;
	return 0;
}