#include <windows.h>

int main(int argc, char* argv)
{
	/*
	Loads inject.dll into the address space of the calling function, in this case the running exe
	*/
	HMODULE dll = LoadLibrary("inject.dll");
	if(dll == NULL)
	{
		printf("Cannot find DLL");
		getchar();
		return -1;
	}

	/*
	Gets the address of the inject method in the inject.dll
	*/
	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "inject");
	if(addr == NULL)
	{
		printf("Cannot find the function");
		getchar();
		return -1;
	}

	/*
	Places a hook in the hookchain for WH_KEYBOARD type events, using the address for the inject method, with the library address
	*/
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
