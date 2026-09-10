#include <iostream>
#include <windows.h>
#include <string>
#include <TlHelp32.h>

DWORD GetProcessIdByName(const std::string &name) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		std::cout << GetLastError();
		return 0;
	}

	PROCESSENTRY32 processEntry{};
	processEntry.dwSize = sizeof(processEntry);

	if (!Process32First(hSnapshot, &processEntry)) {
		std::cout << GetLastError();
		CloseHandle(hSnapshot);
		return 0;
	}

	do {
		if (processEntry.szExeFile == name) {
			CloseHandle(hSnapshot);
			return processEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &processEntry));

	CloseHandle(hSnapshot);
	return 0;
}