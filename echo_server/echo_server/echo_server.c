
#define  WIN                // WIN for Winsock and BSD for BSD sockets


//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#ifdef WIN
#include <winsock2.h>  //Configuring Headersand Libraries for Windows Sockets
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>      // Needed for all Winsock stuff
#endif

#define BUF_SIZE 2019
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0); //Create Socket, Socket function call
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));


	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) //IP addressand port number assignment, bind function call
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR) //Change to connection request, listen function call
		ErrorHandling("listen() error");

	clntAdrSize = sizeof(clntAdr);


	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize); //Accept for connection request, accept function call 
	if (hClntSock == -1)
		ErrorHandling("accept() error");
   else	
	printf("Connected client named damin! \n");


	while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
		send(hClntSock, message, strLen, 0); //	When a socket connection is made, send data using send()

	closesocket(hClntSock);

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
