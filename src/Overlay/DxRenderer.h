#pragma once
#include <d3d11.h>


namespace DxRender {
    extern ID3D11Device*           g_pd3dDevice;
    extern ID3D11DeviceContext*    g_pd3dDeviceContext;
    extern IDXGISwapChain*         g_pSwapChain;
    extern bool                    g_SwapChainOccluded;
    extern UINT                    g_ResizeWidth;
    extern UINT                    g_ResizeHeight;
    extern ID3D11RenderTargetView* g_mainRenderTargetView;

	// Forward declarations of helper functions
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void Present();
}
