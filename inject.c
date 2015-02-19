#include <stdio.h>
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
	MessageBox(NULL, "Injected", "Injected", MB_OK);
	return(CallNextHookEx(NULL, code, wParam, lParam));
}
