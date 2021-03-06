// Make with: gcc -o client.exe client.c -lws2_32 -lmswsock -ladvapi32 -lwinmm

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>
#include <time.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5193"

int iLine;             // Receives line number.
BOOL fRelative;        // Receives check box status. 

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "want_bell";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	int timeout = 0;
	
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    
	iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

	HWND hwndGoto = NULL;  // Window handle of dialog box
    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ) {
			recvbuf[iResult]='\0';
			if(strstr(recvbuf, "BELL") != NULL) {
				printf("Bytes received: %d\n", iResult);
				PlaySound(TEXT("klingel.wav"), NULL, SND_ASYNC);
				
				time_t timer;
				char buffer[50];
				char tmp[60];
				struct tm* tm_info;
				timer = time(NULL);
				tm_info = localtime(&timer);
				strftime(buffer, sizeof(buffer), "%d.%m.%Y um %H:%M:%S Uhr", tm_info);
				snprintf(tmp, sizeof(tmp), "Klingel gedr\201ckt am %s\r\n", buffer);
				printf("%s\n", tmp);
				// Not possible because it's modal and blocks
				//MessageBox(0, "Tuerklingel wurde gedrueckt um", "Tuerklingel",0 | MB_ICONINFORMATION);
			}
			if(strstr(recvbuf, "I") != NULL) {
				
			}
		}
        else if ( iResult == 0 )
            printf("\nConnection closed\n");
        else {
			if(WSAGetLastError() == 10060) {
				timeout = 1;
			}
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
			}
		}

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
	
	if(timeout == 1) {
		printf("Time-Out, restart socket...\n");
		main(argc, argv);
	}

    return 0;
}