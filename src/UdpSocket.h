
#pragma once

#include <windows.h> 

#include <stdint.h>

class UdpSocket {

public:
	UdpSocket(LPTSTR host, uint16_t port);
	~UdpSocket();

	int send(char*, int32_t);

private: 
	SOCKADDR_IN addr;
	SOCKET s;
};