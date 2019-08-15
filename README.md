Convert integer to array of char bytes
======================================
To convert an integer to an array of `char`, you can bitmask against the integer in a loop to get the value for each byte.

Endianness & Bitshifting
------------------------
On a little-endian system, `int 256` is stored across 4 bytes __in memory__ like this:

| Multiplier | 256⁰ | 256¹ | 256² | 256³ |
| -          | -    | -    | -    | -    |
| Value      | 00   | 01   | 00   | 00   |
| Address    | 1000 | 1001 | 1002 | 1003 |

You might imagine that iteratively bitshifting this value and masking against `0xff` would provide the constituent bytes in the little-endian order, but this is not the case.

Bit shifting is always applied against a big-endian representation. This is because when the integer is loaded into the CPU's register, it is effectively represented in big-endian format.

The CPU register is not byte-addressable, so in this context endianness does not make sense.

This means that bitwise operations are applied to a big-endian representation of the number.

```c++

int main()
{
	int num = 256;

	// When the integer is loaded into the CPU's register, it is the 
	// equivalent of conversion to big-endian format - the number in
	// the CPU register is big-endian.
	//
	// This means that bitwise operations are applied to a big-endian
	// representation of the number:
	// 00 00 01 00
	
	std::cout << "num: " << num << '\n';		// 256
	
	std::cout << "(num >> 0) & 0xff = "
		<< (num & 0xff) << '\n';		// 0

	std::cout << "(num >> 8) & 0xff = "
		<< ((num >> 8) & 0xff) << '\n';		// 1

	std::cout << "(num >> 16) & 0xff = "
		<< ((num >> 16) & 0xff) << '\n';	// 0

	std::cout << "(num >> 24) & 0xff = "
		<< ((num >> 24) & 0xff) << '\n';	// 0
	
	return 0;
}
```
In short, you probably don't need to worry about endianness when bit shifting - but bear in mind even if your system is little-endian, the bitshift operation takes place on a big endian representation.

To find endianness on a modern Linux kernel:

```bash
lscpu | grep Endian

# result:
Byte Order:            Little Endian
```

Conversion to an Array of Bytes
-------------------------------

```c++
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
	char buffer[BYTES_PER_INT];
	intToCharArray(buffer, in);
	for (auto& el : buffer) {
		std::cout << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<unsigned int>(el & 0xff) << " ";
	}
	std::cout << '\n';
}
```

References
----------
* [Writing endian-independent code in C][1], IBM Developer article

[1]: https://developer.ibm.com/articles/au-endianc/
