#pragma once

#include <iostream>
#include <Windows.h>

class Memory {
private:
	HANDLE hProcess{};

public:
	Memory(HANDLE hProcess) : hProcess(hProcess) {}

	template <typename T>
	T RPM(uintptr_t lpBaseAddress) {
		T lpBuffer{};

		if (!ReadProcessMemory(
			hProcess,
			reinterpret_cast<LPCVOID>(lpBaseAddress),
			&lpBuffer,
			sizeof(T),
			nullptr
		)) {
			std::cerr << "ReadProcessMemory failed, GetLastError: " << GetLastError() << std::endl;
			return 0;
		}
		return lpBuffer;
	}

	bool bufferRPM(uintptr_t lpBaseAddress, void* lpBuffer, size_t nSize) {
		if (!ReadProcessMemory(
			hProcess,
			reinterpret_cast<LPCVOID>(lpBaseAddress),
			lpBuffer,
			nSize,
			nullptr
		)) {
			std::cerr << "ReadProcessMemory failed. GetLastError: " << GetLastError() << std::endl;
			return false;
		}

		return true;
	}


	//template <typename T>
	//T WPM() {
	//	//return WriteProcessMemory();
	//}
};
