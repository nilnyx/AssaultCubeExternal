#include <format>
#include <iostream>
#include <Windows.h>

#include "ProcessUtils.h"
#include "Constants.h"
#include "Math.h"
#include "Memory.h"
#include "Offsets.h"


int main() {
	DWORD pid = GetProcessIdByName(Constants::processName);
	if (!pid)
		return 1;

	HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
	if (!hProcess)
		return 1;

	Memory mem(hProcess);

	while (true) {
		uintptr_t localPlayer = mem.RPM<uintptr_t>(Addresses::baseAddr + Addresses::localPlayer);
		if (!localPlayer) continue;

		int screenWidth = mem.RPM<int>(Addresses::baseAddr + Addresses::screenWidth);
		int screenHeight = mem.RPM<int>(Addresses::baseAddr + Addresses::screenHeight);
		if (!screenWidth || !screenHeight) continue;

		int playerCount = mem.RPM<int>(Addresses::baseAddr + Addresses::playerCount);
		if (!playerCount) continue;

		uintptr_t entityList = mem.RPM<uintptr_t>(Addresses::baseAddr + Addresses::entityList);
		if (!entityList) continue;

		// 0-31
		for (int i = 0; i < playerCount; i++) {
			uintptr_t entity = mem.RPM<uintptr_t>(entityList + i * 0x4);
			if (!entity) continue;

			char nickname[16];
			mem.bufferRPM(entity + Offsets::nickname, nickname, sizeof(nickname));

			int health = mem.RPM<int>(entity + Offsets::health);
			if (health > 100 || health < 0) continue;

			//std::cout << std::format("{}'s health: {}", nickname, health) << std::endl;

			float feetX = mem.RPM<float>(entity + Offsets::feetPosX);
			float feetY = mem.RPM<float>(entity + Offsets::feetPosY);
			float feetZ = mem.RPM<float>(entity + Offsets::feetPosZ);
			float headY = mem.RPM<float>(entity + Offsets::headPosY);

			Vector3 position = {
				.x = feetX,
				.y = (feetY + headY) / 2,
				.z = feetZ
			};

			float screenX{};
			float screenY{};

			Vector2 screen = {
				.x = screenX,
				.y = screenY
			};

			float viewMatrix[4][4];
			mem.bufferRPM(Addresses::baseAddr + Addresses::viewMatrix, viewMatrix, sizeof(viewMatrix));

			if (!WorldToScreen(position, screen, viewMatrix, screenWidth, screenHeight)) {
				std::cout << "Out of screen" << std::endl;
			} else {
				std::cout << "screen x = " << screen.x << std::endl;
				std::cout << "screen y = " << screen.y << std::endl;
			}
		}


		Sleep(Constants::updateSpeedMs);
	}

	CloseHandle(hProcess);
}
