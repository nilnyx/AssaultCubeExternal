#pragma once

struct Vector2 {
	float x{}, y{};
};

struct Vector3 {
	float x{}, y{}, z{};
};

struct Vector4 {
	float x{}, y{}, z{}, w{};
};


bool WorldToScreen(const Vector3& pos, Vector2& screen, const float matrix[4][4], int screenWidth, int screenHeight);