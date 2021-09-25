#pragma once
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

#pragma warning (disable : 4996)

bool GEN_pNum(int pNum);				// 소수 판독기, 2보다 큰 수가 무조건 입력되기 때문에 2이하 값 판정 필요 없음.
int GEN_N(int p, int q);				// N 생성기
int GEN_LCM(int p, int q);				// 최소공배수 연산 (LCM; 이하 L)
int GEN_GCD(int p, int q);				// 최대공약수 연산 (유클리드 호제법, 모드)
int GEN_E(int LCM, int p, int q);		// E 생성기
int GEN_D(int E, int LCM);				// D 생성기
void KEY_GEN(int* nN, int* nLCM, int* nE, int* nD, int* pr);

bool GEN_pNum(int pNum)					// 소수 판독기, 2보다 큰 수가 무조건 입력되기 때문에 2이하 값 판정 필요 없음.
{
	if ((pNum % 2) == 0)				// 2의 배수들을 제거해줌.
	{
		return false;
	}

	for (int i = 3; i <= (int)sqrt(pNum); i += 2)		// 소수판별법 공식에 따라서 제곱근 범위까지만 3 += 2의 값만 나머지를 판단한다.
	{
		if (pNum % i == 0)
		{
			return false;							// 중간에 나머지가 0이되는 값이 나올 시 소수가 아니게 된다.
		}
	}

	return true;									// i가 루트pNum의 값을 넘어 갈 시 소수이다.
}

int GEN_N(int p, int q)				// N 생성기
{
	int N = p * q;

	return N;
}

int GEN_LCM(int p, int q)			// 최소공배수 연산 (LCM; 이하 L)
{
	int LCM = 0, i = 1;

	if (q > p)
	{
		while (((q * i) % p) != 0)		// 작은 수의 곱이 큰수의 공배수가 될 때 까지 곱해준다.
		{
			if (((q * i) % p) != 0)
			{
				i++;					// 나눠지지 않을 시 곱하는 값을 올려줌
			}
		}
		LCM = q * i;
	}

	else
	{
		while (((p * i) % q) != 0)		// 작은 수의 곱이 큰수의 공배수가 될 때 까지 곱해준다.
		{
			if (((p * i) % q) != 0)
			{
				i++;					// 나눠지지 않을 시 곱하는 값을 올려줌
			}
		}
		LCM = p * i;
	}


	return LCM;
}

int GEN_GCD(int p, int q)			// 최대공약수 연산 (유클리드 호제법, 모드)
{
	if (q == 0)
	{
		return p;
	}

	return GEN_GCD(q, p % q);
}


int GEN_E(int LCM, int p, int q)				// E 생성기
{
	int E = rand() % (p * q - 1) + 1;		// 소수 생성기에서 생성된 p-1과 q-1의 곱이 최대인 범위의 난수.

	while ((GEN_GCD(E, LCM) != 1) || (E <= 1))			// E와 LCM이 서로소가 성립할 때 까지
	{
		E = rand() % (LCM - 1) + 1;	// 1 < E < LCM(p-1, q-1) 의 난수를 생성
	}

	return E;
}

int GEN_D(int E, int LCM)				// D 생성기
{
	int D = LCM / E;					// D의 최솟값은 LCM에서 E를 나눈 몫

	while ((((E * D) % LCM) != 1) || (D < 1))		// E의 곱의역원 D를 연산
	{
		D++;							// 1씩 더해줌
	}
	return D;
}

void KEY_GEN(int* nN, int* nLCM, int* nE, int* nD, int *pr)
{
	srand((unsigned int)time(NULL));
	int p = rand() % 10 + 3, q = rand() % 10 + 3;
	double r = rand() % 10 + 10;
	int LCM, E;

	while (1)			// 난수생성이 제대로 되는가 디버깅
	{
		p = rand() % 150 + 150, q = rand() % 150 + 150;
	//	std::cout << p << " " << q << "\n";										// 연산 과정 디버깅
	//	std::cout << "p = " << p << ", q = " << q << "\n";																//디버깅
	//	std::cout << "GEN_pNum(p) = " << GEN_pNum(p) << ", GEN_pNum(q) = " << GEN_pNum(q) << "\n";						//디버깅
		if ((p != q) && (((p * q) >= 63000) && ((p * q) <= 66000))) {
			if ((GEN_pNum(p) == true) && (GEN_pNum(q) == true))
			{
			//	std::cout << p << " " << q << "\n";								// p q값 소수 디버깅
				break;
			}
		}
	}

	while (true)			// 난수생성이 제대로 되는가 디버깅
	{
		r = rand() % 10 + 10;
		//	std::cout << p << " " << q << "\n";										// 연산 과정 디버깅
		//	std::cout << "p = " << p << ", q = " << q << "\n";																//디버깅
		//	std::cout << "GEN_pNum(p) = " << GEN_pNum(p) << ", GEN_pNum(q) = " << GEN_pNum(q) << "\n";						//디버깅
		if (GEN_pNum(r) == true)
		{
			//	std::cout << p << " " << q << "\n";								// p q값 소수 디버깅
			break;
		}
	}

	*pr = r;
	//std::cout << "p = " << p << ", q = " << q << "\n";																	//디버깅
	//std::cout << "GEN_pNum(p) = " << GEN_pNum(p) << ", GEN_pNum(q) = " << GEN_pNum(q) << "\n\n\n\n\n\n\n\n";			//디버깅
	//std::cout << "소수 난수 두개를 생성합니다.\n\n소수1 = " << p << "소수 2 = " << q << "\n\n";

	*nN = GEN_N(p, q);
	//std::cout << "(p * q) \t\t\t\t N = " << nN << "\n";

	LCM = GEN_LCM(p - 1, q - 1);
	//std::cout << "LCM(p-1, q-1), \t\t\t\t L = " << nLCM << "\n";
	*nLCM = LCM;

	E = GEN_E(GEN_LCM(p - 1, q - 1), p - 1, q - 1);
	//std::cout << "(GCD(E, LCM(p-1, q-1)), \t\t E = " << nE << "\n";
	*nE = E;

	*nD = GEN_D(E, LCM);
	//std::cout << "((E * D) mod LCM = 1) E의 곱셈의 역원 \t D = " << nD << "\n\n";

	// std::cout << p << " " << q << " " << *nN << " " << *nLCM << " " << *nE << " " << *nD << "\n";			// 키생성 디버깅
}