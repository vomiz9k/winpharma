#include "calculator.h"
#include <map>
#include <iostream>
#include <string>
#include <cassert>

int main() {
	Calculator c;
	try {
		std::map<std::string, double> validation = {
			{"1", 1},
			{"2 + 2", 4},
			{"2 * 2", 4},
			{"2 + 2 * 2", 6},
			{"(2 + 2) * 2", 8},
			{"pow(2, 2)", 4},
			{"log(4, 2)", 2},
			{"2.5 * (-2)", -5},
			{"pow((((3 + 5) * 2) - 1), log(4,2))", 225},
		};
		for (const auto& [str, val] : validation) {
			assert(c.process(str) == val);
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
	}
}