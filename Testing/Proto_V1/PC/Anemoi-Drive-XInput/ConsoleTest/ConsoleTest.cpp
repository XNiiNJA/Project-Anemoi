// ConsoleTest.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"


#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include <basetsd.h>
#include <objbase.h>

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <winsock2.h>

#include <Ws2tcpip.h>

#include <chrono>

#include "ControlsHandler.h"

#pragma comment(lib, "ws2_32.lib")

#ifdef USE_DIRECTX_SDK
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\include\xinput.h>
#pragma comment(lib,"xinput.lib")
#elif (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

using namespace std;

HRESULT UpdateControllerState();
void OutputData();

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

#define PORT	6969
#define SERVER "127.0.0.1"

struct CONTROLLER_STATE
{
    XINPUT_STATE state;
    bool bConnected;
};

CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];
WCHAR g_szMessage[4][1024] = {0};
HWND    g_hWnd;
bool    g_bDeadZoneOn = true;

int _tmain(int argc, _TCHAR* argv[])
{

	ControlsHandler c,b;
	float x;
	float y;
	float mag;
	char* buffer;
	size_t * buflen = new size_t(0);


	struct sockaddr_in si_me, si_other;
	sockaddr_in service;
	
	SOCKET dgram_socket;
	int s, slen = sizeof(si_other);
	char broadcast = 0;

	WSADATA wsa; 

	/*Setting up connection*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
	{
		cout << "Error Initializing Winsock";
		Sleep(5000);
		return 1;
	}
	

	addrinfo hints, *results, *res;
	
	ZeroMemory(&hints, sizeof(hints));
	
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(0, "6000", &hints, &results);

	dgram_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	//listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (dgram_socket == INVALID_SOCKET)
	{
		cout << "Error Initializing Socket";
		Sleep(5000);
		WSACleanup();
		return 1;
		//OOOOOOOOOOOOOOOOOOOO DICKS
	}


	/* loop over all returned results and do inverse lookup */
	for (res = results; res != NULL; res = res->ai_next) {
		char hostname[NI_MAXHOST];
		int error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
		if (error != 0) {
			fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
			continue;
		}
		if (*hostname != '\0')
			printf("hostname: %s\n", hostname);
	}


	hostent *thisHost;
	char *ip;
	u_short port;
	


	//Who are we connecting to?
	port = 6000;
	thisHost = gethostbyname("192.168.1.129");
	ip = inet_ntoa(*(struct in_addr *) *thisHost->h_addr_list);

	//AF_INET: Using IPv4 addresses.
	service.sin_family = AF_INET;
	//Setting the ip address we will be sending to.
	service.sin_addr.s_addr = inet_addr(ip);
	//Setting up the port address we will be sending to.
	service.sin_port = htons(port);


	//Bind.
	
	int errorCode = bind(dgram_socket, results->ai_addr, results->ai_addrlen);

	//int errorCode = bind(listenSocket, (SOCKADDR *)& service, sizeof(service));

	if (errorCode == SOCKET_ERROR) {
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		Sleep(5000);
		closesocket(dgram_socket);
		WSACleanup();
		return 1;
	}

	//Now that we've bound, set up settings for the connection.
	errorCode = setsockopt(dgram_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);

	if (errorCode == SOCKET_ERROR) {
		cout << "sockopt error";
		Sleep(5000);
		closesocket(dgram_socket);
		WSACleanup();
		return 1;
	}

	char * arraything = new char[50];

	std::chrono::milliseconds last = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);

	for(;;)
	{

		std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);

		printf("Count: %d\n", now - last);

		last = now;

//		x = (float)(rand() % 254) / (float)5;
//		y = (float)(rand() % 254) / (float)5;

		x = g_Controllers[0].state.Gamepad.sThumbRX;
		y = g_Controllers[0].state.Gamepad.sThumbRY;

		WORD wButtons = g_Controllers[0].state.Gamepad.wButtons;

		bool arm = wButtons & XINPUT_GAMEPAD_A;

		bool disarm = wButtons & XINPUT_GAMEPAD_B;

		x = x / (float)0xFFFF * 2.0f;

		y = y / (float)0xFFFF * 2.0f;

		x = min(1.0f, max(-1.0f, x));

		y = min(1.0f, max(-1.0f, y));


		mag = g_Controllers[0].state.Gamepad.bRightTrigger;

		c.setVector(x,y, 1.0f, mag, Velocity);

		c.setArmState(arm);

		c.setDisarmState(disarm);

		buffer = c.getMessagePackBuffer(buflen);

		for (int i = 0; i < strlen(buffer); i++)
			printf("%d", buffer[i]);

		if (sendto(dgram_socket, buffer, *buflen, 0, (struct sockaddr *) &service, sizeof(service)) == SOCKET_ERROR)
		{
			printf("\nOH BALLS  %d\n", WSAGetLastError());
		}

		UpdateControllerState();
		
		OutputData();

		

		b.putMessagePackBuffer(buffer, *buflen);

		delete buffer; //Delete whatever buffer was pointing to.

	}

}

HRESULT UpdateControllerState()
{
    DWORD dwResult;
    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ )
    {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &g_Controllers[i].state );

        if( dwResult == ERROR_SUCCESS )
            g_Controllers[i].bConnected = true;
        else
            g_Controllers[i].bConnected = false;
    }

    return S_OK;
}

void OutputData()
{
	//WCHAR sz[4][1024];
	WORD wButtons = g_Controllers[0].state.Gamepad.wButtons;

	            printf_s(
                              "Controller 0: Connected\n"
                              "  Buttons: %s%s%s%s%s%s%s%s%s%s%s%s%s%s\n"
                              "  Left Trigger: %u\n"
                              "  Right Trigger: %u\n"
                              "  Left Thumbstick: %d/%d\n"
                              "  Right Thumbstick: %d/%d",
                              ( wButtons & XINPUT_GAMEPAD_DPAD_UP ) ? L"DPAD_UP " : L"",
                              ( wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) ? L"DPAD_DOWN " : L"",
                              ( wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) ? L"DPAD_LEFT " : L"",
                              ( wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) ? L"DPAD_RIGHT " : L"",
                              ( wButtons & XINPUT_GAMEPAD_START ) ? L"START " : L"",
                              ( wButtons & XINPUT_GAMEPAD_BACK ) ? L"BACK " : L"",
                              ( wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) ? L"LEFT_THUMB " : L"",
                              ( wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) ? L"RIGHT_THUMB " : L"",
                              ( wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) ? L"LEFT_SHOULDER " : L"",
                              ( wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) ? L"RIGHT_SHOULDER " : L"",
                              ( wButtons & XINPUT_GAMEPAD_A ) ? L"A " : L"",
                              ( wButtons & XINPUT_GAMEPAD_B ) ? L"B " : L"",
                              ( wButtons & XINPUT_GAMEPAD_X ) ? L"X " : L"",
                              ( wButtons & XINPUT_GAMEPAD_Y ) ? L"Y " : L"",
                              g_Controllers[0].state.Gamepad.bLeftTrigger,
                              g_Controllers[0].state.Gamepad.bRightTrigger,
                              g_Controllers[0].state.Gamepad.sThumbLX,
                              g_Controllers[0].state.Gamepad.sThumbLY,
                              g_Controllers[0].state.Gamepad.sThumbRX,
                              g_Controllers[0].state.Gamepad.sThumbRY );

	//printf("%d", wButtons & XINPUT_GAMEPAD_DPAD_UP);


}