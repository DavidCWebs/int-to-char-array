#include <iostream>
#include <iomanip>
#include <stdio.h>

#define BITS 8

const size_t BYTES_PER_INT = sizeof(int); 

int intToCharArray(char* buffer, int in)
{
	for (size_t i = 0; i < BYTES_PER_INT; i++) {
		size_t shift = 8 * (BYTES_PER_INT - 1 - i);
		buffer[i] = (in >> shift) & 0xff;
	}
	return 0;
}

int main()
{
	std::cout << "Enter an integer: ";
	unsigned int in;
	if (!(std::cin >> in)) {
		std::cout << "Not an integer. Exiting..." << '\n';
		return 1;	
	}
	char buffer[4];
	intToCharArray(buffer, in);
	for (auto& el : buffer) {
		std::cout << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<unsigned int>(el & 0xff) << " ";
	}
	std::cout << '\n';
}
