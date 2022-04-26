#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <string_view>
#include <cassert>

std::vector<double> chundovskiy(int n) {
	std::vector<double> ans(n, 0);
	double fact_6i = 1, fact_3i = 1, fact_i_pow3 = 1;
	double big_number_in_denom = pow(640320., 3);
	for (int i = 0; i < n; ++i) {
		
		double num = ((i % 2) ? -1. : 1.) * 12. * fact_6i * (13591409. + 545140134. * i);
		double den = fact_3i * fact_i_pow3 * pow(big_number_in_denom, (double)i + 0.5);
		ans[i] = num / den;

		for (double j = 6 * i + 1; j <= 6 * (i + 1); ++j) {
			fact_6i *= j;
		}

		for (double j = 3 * i + 1; j <= 3 * (i + 1); ++j) {
			fact_3i *= j;
		}
		fact_i_pow3 *= (double)pow(i + 1, 3);
	}
	return ans;
}

int main() 
{
	auto parts = chundovskiy(10);
	double sum = 0;
	for (int j = parts.size() - 1; j >= 0; --j) {
		sum += parts[j];
	}
	std::cout << std::setprecision(30) << 1 / sum << '\n';
}
