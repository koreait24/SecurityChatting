#pragma once

#include <math.h>
#include <iostream>

#pragma warning (disable : 4996)

unsigned int Decryption(wchar_t c_Text, int prvN_Key, int prvD_Key);

unsigned int Decryption(wchar_t c_Text, int prvN_Key, int prvD_Key)
{
	unsigned int modC = c_Text % prvN_Key;
	unsigned int num = 1;

	for (int i = 0; i < prvD_Key; i++)
	{
		//std::cout << num << " *= " << modC;
		num *= modC;
		//std::cout << " = " << num << "\n";

		//std::cout << num << " %= " << prvN_Key;
		num %= prvN_Key;
		//std::cout << " = " << num << "\n\n";
	}

	//std::cout << "c_Text = " << c_Text << "\t" << "num = " << num << "\n";

	return num;
}