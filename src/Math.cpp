#include "Math.h"


bool WorldToScreen(const Vector3& pos, Vector2& screen, const float matrix[4][4], int screenWidth, int screenHeight) {
	float clipX = matrix[0][0] * pos.x + matrix[1][0] * pos.y + matrix[2][0] * pos.z + matrix[3][0];
	float clipY = matrix[0][1] * pos.x + matrix[1][1] * pos.y + matrix[2][1] * pos.z + matrix[3][1];
	//float clipZ = matrix[0][2] * pos.x + matrix[1][2] * pos.y + matrix[2][2] * pos.z + matrix[3][2]; // unnecessary, if we do 2d esp
	float clipW = matrix[0][3] * pos.x + matrix[1][3] * pos.y + matrix[2][3] * pos.z + matrix[3][3];

	// don't draw if the point is behind camera
	if (clipW <= 0.1f) return false;

	// normalizing device coordinates 
	float ndcX = clipX / clipW;
	float ndcY = clipY / clipW;

	// making sure that NDC is in [-1; 1] range
	if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f) return false;

	// translating NDC to pixels
	screen.x = static_cast<float>(screenWidth) * ((ndcX + 1.0f) / 2.0f);
	screen.y = static_cast<float>(screenHeight) * ((-ndcY + 1.0f) / 2.0f);

	return true;
}