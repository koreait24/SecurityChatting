#pragma once

#include <math.h>
#include <iostream>

#pragma warning (disable : 4996)

unsigned int Encryption(wchar_t p_Text, int pubN_Key, int pubE_Key);

unsigned int Encryption(wchar_t p_Text, int pubN_Key, int pubE_Key)
{
	unsigned int modC = p_Text % pubN_Key;
	unsigned int num = 1;

	for (int i = 0; i < pubE_Key; i++)
	{
		//std::cout << num << " *= " << modC;
		num *= modC;
		//std::cout << " = " << num << "\n";

		//std::cout << num << " %= " << pubN_Key;
		num %= pubN_Key;
		//std::cout << " = " << num << "\n\n";
	}

	//std::cout << "p_Text = " << p_Text << "\t" << "num = " << num << "\n";

	return num;
}