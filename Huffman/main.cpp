#include <iostream>
#include "huffman.h"
#include <limits>

int main()
{
	std::cout << "Example test: test.bmp -> test.hrar -> new_test.bmp" << std::endl << std::endl;
	Huffman_encode("test.bmp", "test.hrar");
	Huffman_decode("test.hrar", "new_test.bmp");
	char c;
	do
	{
		std::cout << "Enter a command(c - compress, d - decompress, e - exit): ";
		std::cin >> c;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if( (c == 'c') || (c == 'd') )
		{
			std::cout << "Enter the input's file name:\n";
			char* in = new char[256];
			std::cin.getline(in, 256);
			std::cout << "Enter the output's file name:\n";
			char* out = new char[256];
			std::cin.getline(out, 256);
			if(c == 'c')
				Huffman_encode(in, out);
			else
				Huffman_decode(in, out);
		}
	}while( (c == 'c') || (c == 'd') );
	std::system("pause");
	return 0;
}