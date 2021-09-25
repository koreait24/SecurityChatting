#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "Client.h"
#include "Server.h"
#include "RSA_KEY_GEN.h"
#include "DH_KeyC.h"

using namespace std;

#pragma warning (disable : 4996)

#pragma comment( lib , "ws2_32.lib" )

int main(int q, int pri_Root, int nD, int nE, int nN, int nLCM);
string Menu(int *q, int *pri_Root, int *nD, int *nE, int *nN, int *nLCM);							// 계정 등록과 없을시 가입을 요구하는 함수 / return 값 유저 네임
void Register();						// 계정을 등록하는 창 / ofstream으로 user.prof파일을 저장함
string Log_in(string name, int* q, int* pri_Root, int* nD, int* nE, int* nN, int* nLCM);					// 로그인 기능을 하는 창 / ifstream으로 user.prof파일을 읽어옴


int main(int q, int pri_Root, int nD, int nE, int nN, int nLCM)
{
	string name;
	int select = 0;
	char ip_addr[] = "\0";
	char nickname[] = ": ";

	name = Menu(&q, &pri_Root, &nD, &nE, &nN, &nLCM);
	Log_in(name, &q, &pri_Root, &nD, &nE, &nN, &nLCM);

	std::cout << "\n=========================================================\n";
	std::cout << "=\t\t유저정보를 조회합니다.\t\t\t=\n";
	std::cout << "=\t\t로그인 정보 : " << name << "\t\t\t=";
	std::cout << "\n=========================================================\n";

	const char *name_in = name.c_str();				// string 타입의 name을 const char* 타입으로 형변환 / 매개변수 형태 맞춰주기

	// std::cout << "String의 값은 " << name << endl;

	while (1)
	{
		std::cout << "\n=========================================================\n";
		std::cout << "=\t\t1. 채팅방을   생성한다.\t\t\t=\n=\t\t2. 채팅방에   접속한다.\t\t\t=\n=\t\t3. 프로그램을 종료한다.\t\t\t=";
		std::cout << "\n=========================================================\n";
		cin >> select;

		std::cout << name << " " << q << " " << pri_Root << " " << nN << " " << nLCM << " " << nE << " " << nD << "\n";

		if (select == 1)
		{
			system("cls");

			std::cout << name_in << "님이 서버를 생성합니다....(/help 사용시 명령어가 등장합니다)\n\n";

			strcat((char*)name_in, nickname);				// 이름에서 채팅창 닉네임으로 폼을 바꾸기 위한 틀 / "조건섭" -> "조건섭: "

			Server((wchar_t*)name_in, q, pri_Root, nD, nE, nN, nLCM);					// Server의 name 매개변수가 char형이므로 const char* 에서 char* 로 형변환
		}

		else if (select == 2)
		{
			std::cout << "접속할 IP주소를 입력해주세요\n";
			cin >> ip_addr;

			system("cls");

			std::cout << name_in << "님으로" << ip_addr << "서버에 접속합니다....(/help 사용시 명령어가 등장합니다)\n\n";

			strcat((char*)name_in, nickname);				// 이름에서 채팅창 닉네임으로 폼을 바꾸기 위한 틀 / "조건섭" -> "조건섭: "

			Client((wchar_t*)name_in, ip_addr, q, pri_Root, nD, nE, nN, nLCM);		// Client의 name 매개변수가 char형이므로 const char* 에서 char* 로 형변환
		}

		else if (select == 3)
		{
			std::cout << "\n\n프로그램을 종료합니다....\n\n";
			break;
		}

		else
		{
			std::cout << "\n\n잘못된 입력입니다....\n\n";
		}
	}

	system("Pause");

	return 0;
}

string Menu(int *q, int *pri_Root, int *nD, int *nE, int *nN, int *nLCM)									// 메뉴 세션 / 계정등록을 진행하고 계정이 없을시 계정등록을 유도함
{
	int mq = 0, mpri_Root, mnD, mnE, mnN, mnLCM;

	string name;

	int input = NULL;
	std::cout << "\n=========================================================\n";
	std::cout << "=\t기존에 등록한 계정이 있으십니까? (1.yes/2.no)\t=";
	std::cout << "\n=========================================================\n";

	while (1) {
		if (mq != 0)
		{
			return name;
		}

		cin >> input;

		if (input == 1)
		{
			std::cout << "\n=========================================================\n";
			std::cout << "=\t\t로그인을 진행합니다.\t\t\t=";
			std::cout << "\n=========================================================\n";
			name = Log_in(name, &mq, &mpri_Root, &mnD, &mnE, &mnN, &mnLCM);

			Log_in(name, &mq, &mpri_Root, &mnD, &mnE, &mnN, &mnLCM);

			return name;
		}

		else if (input == 2)
		{
			std::cout << "\n=========================================================\n";
			std::cout << "=\t\t계정등록을 진행합니다.\t\t\t=";
			std::cout << "\n=========================================================\n";
			Register();
		}

		else
		{
			std::cout << "\n=========================================================\n";
			std::cout << "=\t\t잘못된 입력입니다.\t\t\t=";
			std::cout << "\n=========================================================\n";
		}
	}

}

void Register()								// 계정 등록 세션
{
	char buf[20] = "\0";
	int count = 0;

	int nD, nE, nN, nLCM;
	int q;
	int pub_Key = 0, pri_Root = 0, o_pub_key = 0;

	KEY_GEN(&nN, &nLCM, &nE, &nD, &q);
	pri_Root = prim_Root(q);

	//std::cout << nN << " " << nLCM << " " << nE << " " << nD << " " << q << " " << pri_Root << "\n";			// 키생성 디버깅

	ofstream ofs;
	ofs.open("user.prof");

	std::cout << "\n=========================================================\n";
	std::cout << "=\t\t계정을 생성합니다.\t\t\t=\n";
	std::cout << "=\t\t이름을 입력해주세요.\t\t\t=";
	std::cout << "\n=========================================================\n";

	cin >> buf;

	ofs << buf << "\n";;
	ofs << q << "\n";
	ofs << pri_Root << "\n";
	ofs << nN << "\n";
	ofs << nLCM << "\n";
	ofs << nE << "\n";
	ofs << nD << "\n";

	std::cout << "계정이 생성되었습니다.\n";

	ofs.close();

	main(q, pri_Root, nD, nE, nN, nLCM);
}

string Log_in(string name, int* q, int* pri_Root, int* nD, int* nE, int* nN, int* nLCM)					// 계정 로그인 세션 / 파일이 손상 되었거나 옳지 않은 파일일 경우 재등록 요구
{
	ifstream ifs;

	ifs.open("user.prof");
	getline(ifs, name);

	if (ifs.is_open() && name[0] != NULL) {

		for (int i = 1; i < 7; i++)
		{

			if (i == 1) {
				ifs >> *q;
			}

			if (i == 2) {
				ifs >> *pri_Root;
			}

			if (i == 3) {
				ifs >> *nN;
			}

			if (i == 4) {
				ifs >> *nLCM;
			}

			if (i == 5) {
				ifs >> *nE;
			}

			if (i == 6) {
				ifs >> *nD;
			}
		}

		// std::cout << name << " " << *q << " " << *pri_Root << " " << " " << *nN << " " <<*nLCM << " " << *nE << " " << *nD;

		return name;

		ifs.close();
	}

	else if (name[0] == NULL) {
		std::cout << "\n\n\n계정정보가 손상되었습니다. 다시 계정생성 해주시기 바랍니다.\n\n\n";

		Register();

		ifs.close();
	}

	else {
		std::cout << "\n\n\n계정생성을 먼저 해주시기 바랍니다.\n\n\n";

		Register();

		ifs.close();
	}
}