#pragma once
#include "DxRenderer.h"
#include "imgui.h"


namespace Overlay {
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void InitImGuiContext(HWND hwnd, float main_scale);
	void BeginImGuiFrame();
	void RenderImGui();
	void UI();
	void Cleanup();

	bool Start();
}
