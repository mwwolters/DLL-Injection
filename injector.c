#include <windows.h>

int main(int argc, char* argv)
{

	HMODULE dll = LoadLibrary("inject.dll");
	if(dll == NULL)
	{
		printf("Cannot find DLL");
		getchar();
		return -1;
	}

	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "inject");
	if(addr == NULL)
	{
		printf("Cannot find the function");
		getchar();
		return -1;
	}

	HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, addr, dll, 0);
	if(handle == NULL)
	{
		printf("Couldn't hook the keyboard");
	}

	printf("Hooked the program, hit enter to exit");
	getchar();
	UnhookWindowsHookEx(handle);

	return 0;
}
