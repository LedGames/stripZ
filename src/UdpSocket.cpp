#pragma comment(lib, "Ws2_32.lib")

#include "UdpSocket.h"

	UdpSocket::UdpSocket(LPTSTR host, uint16_t port) {

		WSADATA wsa;
		int ret = WSAStartup(MAKEWORD(2, 0), &wsa);
		if (ret != 0) {
			int wsaErr = WSAGetLastError();

			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, wsaErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		}

		s = socket(AF_INET, SOCK_DGRAM, 0);

		memset(&addr, 0, sizeof(SOCKADDR_IN));  

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);  
		
		char buf[50];
		int ccnt = wcstombs ( buf, host, sizeof(buf) );
		buf[ccnt] = '\0';

		addr.sin_addr.s_addr = inet_addr(buf); 

		ret = connect(s, (SOCKADDR*)&addr, sizeof(SOCKADDR));
		if (ret != 0) {
			int wsaErr = WSAGetLastError();

			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, wsaErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		}

	}

	int UdpSocket::send(char* buf, int32_t len) {
		return sendto(s, buf, len, 0, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
			 
	}

	UdpSocket::~UdpSocket() {
		WSACleanup();
	}
	 