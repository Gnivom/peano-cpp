#include "Peano.h"

#include <iostream>

int main() {
	using namespace Peano;
	std::cout << toInt(Three * Five - Two) << std::endl;
}