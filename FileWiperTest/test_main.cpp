#include "../FileWiper.h"
#include <iostream>

int main()
{
	FileWiper wiper_zero("C:/Users/MathxH/Desktop/wipertest/test.dat", FileWiper::WipeMode::ZEROES);

	if (!wiper_zero.start())
	{
		std::cout << " zeros process failed" << std::endl;
	}

	wiper_zero.setFile("C:/Users/MathxH/Desktop/wipertest/test.dat");
	wiper_zero.setWipeMode(FileWiper::WipeMode::PSEUDO_RANDOM);

	if (!wiper_zero.start())
	{
		std::cout << " pseudo random process failed" << std::endl;
	}
	
	return 0;
}