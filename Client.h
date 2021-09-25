#pragma once

#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <string>

#include "Cursor.h"
#include "RSA_KEY_GEN.h"
#include "Decryption.h"
#include "Encryption.h"
#include "DH_KeyC.h"

#pragma comment( lib , "ws2_32.lib" )

#pragma warning (disable : 4996)

using namespace std;

void ErrorHandling_C(const char *message);

int Client(wchar_t* name, char ip_addr[], int q, int pri_Root, int nD, int nE, int nN, int nLCM)
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	wchar_t message[1000];
	int strLen;
	int unstrLen;
	int flag = 0;
	int cur_Y = 5;
	int init = 0;
	unsigned int crypto_Len;
	unsigned int decryp_Len;
	
	int pub_Key = 0, o_pub_Key = 0, secret_Key = 0, nE_Key = 0, nN_Key = 0;

	bool key_status = false;

	u_long iMode = 1;

	wchar_t* save_Name = name;

	name = (wchar_t*)malloc(sizeof(wchar_t) * 30);


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리를 초기화하고 있다

		ErrorHandling_C("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //소켓을 생성하고

	strLen = ioctlsocket(hSocket, FIONBIO, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling_C("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip_addr);
	servAddr.sin_port = htons(16001);

	ofstream ofs;
	ofs.open("chat_report.txt");

	while (1)
	{
		if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다
			ErrorHandling_C("connect() error!");

		if ((WSAGetLastError() != WSAEWOULDBLOCK) || (hSocket != INVALID_SOCKET))
		{
			break;
		}
	}

	while (1)
	{

		if (!key_status)
		{
			strLen = recv(hSocket, (char*)message, 1000, flag); //서버로 부터 소수 q와 원시근 pri_Root를 받아온다.
			q = atoi((char*)message);
			strLen = recv(hSocket, (char*)message, 1000, flag);
			pri_Root = atoi((char*)message);
			strLen = recv(hSocket, (char*)message, 1000, flag); //recv 함수 호출을 통해서 서버로부터 키교환을 하고 있다.

			std::cout << "q : " << q << "/ p_Root : " << pri_Root << "\n";

			if (strLen == -1)
				ErrorHandling_C("read() error");

			pub_Key = PubK_Calc(q, pri_Root, nD);		// 받은 정보들로 공개키 생성

				//std::cout << nN << " " << nLCM << " " << nE << " " << nD << " " << q << " " << pri_Root << "\n";			// 키생성 디버깅

			std::cout << "생성한 키 교환 값 : (" << pri_Root << " ^ " << nD << ") mod " << q << " = " << pub_Key << "\n";

			o_pub_Key = atoi((char*)message);
			secret_Key = DH_KeyC(q, nD, o_pub_Key);

			send(hSocket, itoa(pub_Key, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도

			strLen = recv(hSocket, (char*)message, 1000, flag); //recv 함수 호출을 통해서 서버로부터 키교환을 하고 있다.
			nE_Key = atoi((char*)message);
			send(hSocket, itoa(nE, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도

			strLen = recv(hSocket, (char*)message, 1000, flag); //recv 함수 호출을 통해서 서버로부터 키교환을 하고 있다.
			nN_Key = atoi((char*)message);
			send(hSocket, itoa(nN, (char*)message, 10), 1000, flag); //send함수 호출을 통해서 연결된 서버에 키교환 시도

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				std::cout << "수신한 키 : " << o_pub_Key << " | 송신한 키 : " << pub_Key << "\n";
				std::cout << "시크릿 키 : (" << o_pub_Key << " ^ " << nD << ") mod " << q << " = " << secret_Key << "\n";
				std::cout << "상대방의 공개키 E : " << nE_Key << "\n";
				std::cout << "상대방의 공개키 N : " << nN_Key << "\n";
				cur_Y += 5;
			}
			key_status = true;							// 키교환 완료 후 일반채팅으로 전환
			Sleep(100);
		}
		//------------------------------------------------------------------------------------------------------------------------------------
		while (1)
		{
			*message = 0;

			strLen = recv(hSocket, (char*)message, 1000, flag); //recv 함수 호출을 통해서 서버로부터 전송되는 데이터를 수신하고 있다.
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
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
			
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

			//-----------------------------------------------------------------------------------------------------------------------------------
			std::cout << "복호화된 암호문 : " << (char*)message << "\n";
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

			ofs << (char*)message << "\n";

			cur_Y++;

			if (strLen == -1)
				ErrorHandling_C("read() error");

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				break;
			}

		}

		for (init = 2; init <= strlen((char*)message); init++)
		{
			if (((char)message[init - 2] == '/') && ((char)message[init - 1] == 'x') && ((char)message[init] == 't'))
			{
				std::cout << "상대방이 채팅을 종료하였습니다.\n";

				closesocket(hSocket); //소켓 라이브러리 해제
				WSACleanup();

				free(name);

				system("pause");

				ofs.close();

				return 0;
			}
		}


		//-----------------------------------------------------------------------------------------------------------------------------------
		*message = NULL;
		//------------------------------------------------------------------------------------------------------------------------------------

		while (1)
		{
			*message = 0;
			cin.sync();
			cin.getline((char*)message, 1000);

			/*gotoxy(strlen((char*)message), cur_Y);							잡기능 잠시 제외


			cur_Y++;

			gotoxy(strlen((char*)message), cur_Y);

			for (init = 0; init < strlen((char*)name); init++) {
				std::cout << "\b";
			}

			init = 0;
			gotoxy(0, cur_Y);*/

			strcpy((char*)name, (char*)save_Name);
			strcat((char*)name, (char*)message);


			ofs << (char*)name << "\n";
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
			for (init = 0; init < strlen((char*)name); init++)
			{
				std::cout << "\tp_Text[" << init << "] = " << (int)name[init] << "\t";
				if (init % 3 == 0)
				{
					std::cout << "\n";
				}
			}

			std::cout << "\n";
			std::cout << "만든 평문 : " << (char*)name << "\n";		   
			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

			//------------------------------------------------------------------------------------------------------------------------------------

			std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

			for (init = 1; init < strlen((char*)name); init++)
			{
				crypto_Len = Encryption(name[init - 1], nN_Key, nE_Key);
				name[init - 1] = crypto_Len;
				std::cout << "\tc_Text[" << init - 1 << "] = " << (int)name[init - 1] << "\t";

				if (init % 3 == 0)
				{
					std::cout << "\n";
					cur_Y++;
				}
			}
			std::cout << "\n";

			cur_Y++;

			strLen = send(hSocket, (char*)name, 1000, flag); //send함수 호출을 통해서 연결된 서버에 데이터를 전송 / name + message라서 name의 형태로 전송

			if ((WSAGetLastError() != WSAEWOULDBLOCK) || (strLen != SOCKET_ERROR))
			{
				std::cout << "보낸 암호문 : " << (char*)name << "\n";
				std::cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
				cur_Y++;
				break;
			}
		}

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
	}

	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();

	free(name);

	system("pause");

	ofs.close();

	return 0;
}

void ErrorHandling_C(const char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}