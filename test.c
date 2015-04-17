#include <stdio.h>
#include <windows.h>
int test()
{
	char str[80];
	int id = GetCurrentProcessId();
	sprintf(str, "Hello, process: %d", id);
	MessageBox(NULL, str, "Hello DLL!", MB_OK);
	return 0;
}

