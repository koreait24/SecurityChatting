#pragma once

#include <math.h>
#include <iostream>

#pragma warning (disable : 4996)

int DH_KeyC(int q, int prv_Key, int pub_Key);
int PubK_Calc(int q, int a, int prv_Key);
int prim_Root(int r);

int DH_KeyC(int q, int prv_Key, int pub_Key)
{
	int secret_Key;

	int modC = pub_Key % q;
	int num = 1;
	
	for (int i = 0; i < prv_Key; i++)
	{
		num *= modC;
		num %= q;
	}

	secret_Key = num;

	return secret_Key;			// Diffe-Hellman 키교환 Secret Key
}

int PubK_Calc(int q, int a, int prv_Key)
{
	int pub_Key;

	int modC = a % q;
	int num = 1;

	for (int i = 0; i < prv_Key; i++)
	{
		num *= modC;
		num %= q;
	}

	pub_Key = num;

	return pub_Key;
}

int prim_Root(int r)
{
	int jud_num = 0, count = 0;
	double i, j;
	double now_num = 0;

	for (i = 3.0; i < r; i++)
	{
		count = 0;
		jud_num = 0;
		for (j = 1.0; j < r; j++)
		{
			now_num = fmod(pow(i, j), r) + 0.5;

			std::cout << "pow" << i << " " << j << " " << r << " " << (int)now_num << "\n";	// 디버깅
			count += (int)now_num;
			jud_num += j;

			if ((j == (r - 1)) && (count == jud_num))
			{
				return i;
			}
		}

		std::cout << jud_num << " " << count << "\n";										// 디버깅
	}

	return i;
}
