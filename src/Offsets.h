#pragma once
#include <cstdint>


namespace Addresses {
	constexpr uintptr_t baseAddr = 0x400000;
	constexpr uintptr_t localPlayer = 0x17E0A8;
	constexpr uintptr_t entityList = 0x18AC04;
	constexpr uintptr_t playerCount = 0x18AC0C;
	constexpr uintptr_t viewMatrix = 0x17DFD0;
	constexpr uintptr_t screenWidth = 0x191ED8;
	constexpr uintptr_t screenHeight = 0x191EDC;
	constexpr uintptr_t fov = 0x18A7CC;
}

namespace Offsets {
	constexpr uintptr_t headPosX = 0x04;
	constexpr uintptr_t headPosY = 0x08;
	constexpr uintptr_t headPosZ = 0x0C;

	constexpr uintptr_t feetPosX = 0x28;
	constexpr uintptr_t feetPosY = 0x2C;
	constexpr uintptr_t feetPosZ = 0x30;

	constexpr uintptr_t viewX = 0x34;
	constexpr uintptr_t viewY = 0x38;

	constexpr uintptr_t health = 0xEC;
	constexpr uintptr_t armor = 0xF0;
	constexpr uintptr_t riffleAmmo = 0x140;
	constexpr uintptr_t fire = 0x204;
	constexpr uintptr_t nickname = 0x205;

}
