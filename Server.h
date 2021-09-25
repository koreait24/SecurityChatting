#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <string>

#include "Cursor.h"
#include "RSA_KEY_GEN.h"
#include "Decryption.h"
#include "Encryption.h"
#include "DH_KeyC.h"

#pragma comment( lib , "ws2_32.lib" )

#pragma warning (disable : 4996)

using namespace std;

void ErrorHandling_S(const char *message);

int Server(wchar_t* name, int q, int pri_Root, int nD, int nE, int nN, int nLCM)						// name값을 넘겨받아 message 첫자리에 strcat()를 이용해 결합
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int strLen;
	int szClntAddr;
	int count = 0;
	wchar_t message[1000];
	char buf[] = "\0";
	int cur_Y = 7;
	int flag = 0;
	int init = 0;
	unsigned int crypto_Len;
	unsigned int decryp_Len;

	int pub_Key = 0, o_pub_Key = 0, secret_Key = 0, nE_Key = 0, nN_Key = 0;

	bool key_status = false;

	u_long iMode = 1;

	wchar_t* save_Name = name;
	wchar_t* full_message;

	name = (wchar_t*)malloc(sizeof(wchar_t) * 30);

	// std::cout << save_Name << " 이름\n"; // save_Name 넘어오는거 디버깅

	pub_Key = PubK_Calc(q, pri_Root, nD);

	std::cout << "q : " << q << "/ p_Root : " << pri_Root << "\n";
	std::cout << "생성한 키 교환 값 : (" << pri_Root << " ^ " << nD << ") mod " << q << " = " << pub_Key << "\n";

	std::cout << "지정된 포트로 서버를 생성합니다..\n\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리 초기화

		ErrorHandling_S("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0); //소켓생성

	if (hServSock == INVALID_SOCKET)
		ErrorHandling_S("socket() error");


	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(16001);


	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //소켓에 IP주소와 PORT 번호 할당
		ErrorHandling_S("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR) //listen 함수호출을 통해서 생성한 소켓을 서버 소켓으로 완성
		ErrorHandling_S("listen() error");

	szClntAddr = sizeof(clntAddr);

	ofstream ofs;
	ofs.open("chat_report.txt");

	while (1)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //클라이언트 연결요청 수락하기 위해 accept함수 호출

		if ((WSAGetLastError() != WSAEWOULDBLOCK) || (hClntSock != INVALID_SOCKET))
		{
			break;
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------

	while (1)
	{

		strLen = ioctlsocket(hClntSock, FIONBIO, 0);

		*message = 0;
		

		if (!key_status)
		{

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				strLen = send(hClntSock, itoa(q, (char*)message, 10), 1000, flag); //소수 q와 원시근 pri_Root 값 전송
				strLen = send(hClntSock, itoa(pri_Root, (char*)message, 10), 1000, flag);
				strLen = send(hClntSock, itoa(pub_Key, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도
				strLen = recv(hClntSock, (char*)message, 1000, flag); //recv 함수 호출을 통해서 클라이언트로부터 키를 수신받고 있다.
				o_pub_Key = atoi((char*)message);
				secret_Key = DH_KeyC(q, nD, o_pub_Key);

				strLen = send(hClntSock, itoa(nE, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도
				strLen = recv(hClntSock, (char*)message, 1000, flag); //recv 함수 호출을 통해서 클라이언트로부터 키를 수신받고 있다.
				nE_Key = atoi((char*)message);

				strLen = send(hClntSock, itoa(nN, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도
				strLen = recv(hClntSock, (char*)message, 1000, flag); //recv 함수 호출을 통해서 클라이언트로부터 키를 수신받고 있다.
				nN_Key = atoi((char*)message);

				std::cout << "송신한 키      : " << pub_Key << " | 수신한 키 : " << o_pub_Key << "\n";
				std::cout << "시크릿 키      : (" << o_pub_Key << " ^ " << nD << ") mod " << q << " = " << secret_Key << "\n";
				std::cout << "상대방 공개키 E: " << nE_Key << "\n";
				std::cout << "상대방 공개키 N: " << nN_Key << "\n";
				cur_Y += 4;
			}
			key_status = true;							// 키교환 완료 후 일반채팅으로 전환
			Sleep(100);
		}
		//------------------------------------------------------------------------------------------------------------------------------------
		init = 0;
		
		while (1)
		{
			*message = 0;
			cin.sync();
			cin.getline((char*)message, 1000);

			/*gotoxy(strlen((char*)message), cur_Y);					잡기능 잠시 제외

			for (init = sizeof(name[init]); init < strlen((char*)message); init++) {
				std::cout << "\b";
			}
			init = 0;

			gotoxy(0, cur_Y);*/
			
			strcpy((char*)name, (char*)save_Name);
			full_message = (wchar_t*)strcat((char*)name, (char*)message);

			cur_Y++;

			ofs << (char*)name << "\n";

			
			cur_Y++;

			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
			for (init = 0; init < strlen((char*)full_message); init ++)
			{
				std::cout << "\tp_Text[" << init << "] = " << (int)full_message[init] << "\t";
				if (init % 3 == 0)
				{
					std::cout << "\n";
				}
			}

			std::cout << "\n";

			std::cout << "만든 평문 : " << (char*)full_message << "\n";

			
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

			for (init = 1; init < strlen((char*)full_message); init ++)
			{
				crypto_Len = Encryption(full_message[init - 1], nN_Key, nE_Key);
				full_message[init - 1] = crypto_Len;
				std::cout << "\tc_Text[" << init - 1 << "] = " << (int)full_message[init - 1] << "\t";

				if (init % 3 == 0)
				{
					std::cout << "\n";
					cur_Y++;
				}
			}
			std::cout << "\n";

			cur_Y++;

			strLen = send(hClntSock, (char*)full_message, 1000, flag); //send함수 호출을 통해서 연결된 서버에 데이터를 전송 / name + message라서 name의 형태로 전송

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				std::cout << "보낸 암호문 : " << (char*)full_message << "\n";
				std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
				cur_Y++;
				break;
			}
			
		}

		//std::cout << name << "\n";			//name 값 초기화 확인용
		//std::cout << message << "\n";		//message 값 초기화 확인용

		if (hClntSock == INVALID_SOCKET)
			ErrorHandling_S("accept() error");
		//------------------------------------------------------------------------------------------------------------------------------------
		if (!strcmp((char*)message, "/exit"))
		{
			std::cout << "알림 : 채팅을 종료합니다..\n\n";
			break;
		}

		else if (!strcmp((char*)message, "/clear"))
		{
			std::cout << "알림 : 채팅창을 정리합니다..\n\n";
			system("cls");
			cur_Y = 1;
		}

		else if (!strcmp((char*)message, "/help"))
		{
			std::cout << "알림 : 도움말 기능입니다..\n";
			std::cout << "/help  : 도움말을   불러옵니다\n";
			std::cout << "/exit  : 채팅을     종료합니다\n";
			std::cout << "/clear : 채팅창을   청소합니다.\n";
			cur_Y += 4;
		}
		//------------------------------------------------------------------------------------------------------------------------------------
		*message = 0;

		while (1)
		{
			strLen = recv(hClntSock, (char*)message, 1000, flag); //recv 함수 호출을 통해서 클라이언트로부터 전송되는 데이터를 수신하고 있다.


			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
			for (init = 1; init < strlen((char*)message); init++)
			{
				decryp_Len = message[init - 1];

				std::cout << "\tc_Text[" << init - 1 << "] = " << (int)message[init - 1] << "\t";
				if (init % 3 == 0)
				{
					std::cout << "\n";
					cur_Y++;
				}
			}
			std::cout << "\n";

			std::cout << "받은 암호문 : " << (char*)message << "\n";
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

	
			//-----------------------------------------------------------------------------------------------------------------------------------

			for (init = 1; init < strlen((char*)message); init++)
			{

				decryp_Len = Decryption(message[init - 1], nN, nD);
				message[init - 1] = decryp_Len;

				std::cout << "\tp_Text[" << init - 1 << "] = " << (int)message[init - 1] << "\t";
				if (init % 3 == 0)
				{
					std::cout << "\n";
					cur_Y++;
				}
			}
			std::cout << "\n";

			std::cout << "복호화된 암호문 : " << (char*)message << "\n";
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

			ofs << (char*)message << "\n";

			cur_Y++;

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				break;
			}
				
		}
		//------------------------------------------------------------------------------------------------------------------------------------

				//-----------------------------------------------------------------------------------------------------------------------------------
		for (init = 4; init <= strlen((char*)message) - 4; init++)
		{
			if (((char)message[init - 2] == '/') && ((char)message[init - 1] == 'x') && ((char)message[init] == 't'))
			{
				std::cout << "상대방이 채팅을 종료하였습니다.\n" << endl;

				closesocket(hClntSock); //소켓 라이브러리 해제
				WSACleanup();

				free(name);

				system("pause");

				ofs.close();

				return 0;
			}
		}
	}
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup(); //프로그램 종료 전에 초기화한 소켓 라이브러리 해제

	free(name);

	system("pause");

	ofs.close();

	return 0;
}

void ErrorHandling_S(const char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
