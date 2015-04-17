#include <windows.h>

typedef int (*importFunction)();

int loadDLL()
{
	int status = 0;
	importFunction test;
	HINSTANCE testLibrary = LoadLibrary("test.dll");
	if (testLibrary)
	{
		test = (importFunction)GetProcAddress(testLibrary, "test");
		if(test)
		{
			status = test();
		}
		FreeLibrary(testLibrary);
	}
	return status;
}

int main(int argc, char **argv)
{
	loadDLL();
}
