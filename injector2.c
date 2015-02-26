#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>

DWORD getThreadID(DWORD pid)
{
	puts("Getting Thread ID");
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if(h != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if( Thread32First(h, &te))
		{
			do
			{
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
				{
					if(te.th32OwnerProcessID == pid)
					{
						HANDLE hThread = OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);
						if(!hThread)
						{
							puts("Couldn't get thread handle");
						}
						else
						{
							//DWORD tpid = GetProcessIdOfThread(hThread);
							//printf("Got one: %u\n", tpid);
							return te.th32ThreadID;
						}
					}
				}
			} while( Thread32Next(h, &te));
		}
	}
	CloseHandle(h);
	return (DWORD)0;
}

int processInject(int pid)
{
	DWORD processID = (DWORD)pid;

        TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

        if (NULL != hProcess)
        {
                HMODULE hMod;
                DWORD cbNeeded;

                if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
                {
                        GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
                }
        }

	_tprintf( TEXT("Injecting into process %s PID: %u\n"), szProcessName, processID);

	DWORD threadID = getThreadID(processID);

	printf( "Using Thread ID %u\n", threadID);

	if(threadID == (DWORD)0)
	{
		puts("Cannot find thread");
		return -1;
	}

	HMODULE dll = LoadLibrary("inject2.dll");
	if(dll == NULL)
	{
		puts("Cannot find DLL");
		return -1;
	}

	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "test");
	if(addr == NULL)
	{
		puts("Cannot find the function");
		return -1;
	}

	HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, addr, dll, threadID);

	if(handle == NULL)
	{
		puts("Couldn't hook the keyboard");
	}
	HANDLE hPid = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	puts("Sending message to process to hook");
	SendMessage(hPid,0,0,0);
	getchar();
	getchar();
	getchar();
	UnhookWindowsHookEx(handle);
	return 0;
}

int main(int argc, char* argv)
{

	int pid;
	puts("Inject into which PID?");
        scanf ("%u",&pid);
	printf("PID entered: %u\n", pid);
	int result = processInject(pid);
	if(result == -1)
	{
		puts("Could not inject");
	}
	else
	{
		puts("Injected!");
	}
	getchar();
}
