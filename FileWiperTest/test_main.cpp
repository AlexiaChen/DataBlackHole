#include "../FileWiper.h"
#include <iostream>

int main()
{
	FileWiper wiper("C:/Users/MathxH/Desktop/wipertest/test.dat", FileWiper::WipeMode::ZEROES);

	if (!wiper.start())
	{
		std::cout << "process failed" << std::endl;
	}
	
	return 0;
}