#include "Overlay.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

void Overlay::UI() {
    ImGui::Begin("Test");

    ImGui::End();
}

void Overlay::RenderImGui() {
    UI();

    ImGui::Render();
    const float clear_color_with_alpha[4] =
    {
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    DxRender::g_pd3dDeviceContext->OMSetRenderTargets(1, &DxRender::g_mainRenderTargetView, nullptr);
    DxRender::g_pd3dDeviceContext->ClearRenderTargetView(DxRender::g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::InitImGuiContext(HWND hwnd, float main_scale) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(DxRender::g_pd3dDevice, DxRender::g_pd3dDeviceContext);
}

void Overlay::BeginImGuiFrame() {
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Overlay::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        DxRender::g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        DxRender::g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool Overlay::Start() {
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = {
	    sizeof(wc),
    	CS_CLASSDC,
    	WndProc,
    	0L, 0L,
    	GetModuleHandle(nullptr),
    	nullptr,
    	nullptr,
    	nullptr,
    	nullptr,
    	L"ImGui Example",
    	nullptr
    };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW ,
        wc.lpszClassName,
        L"Dear ImGui DirectX11 Example",
        WS_POPUP,
        0, 0,
        1920,
        1080,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    // Initialize Direct3D
    if (!DxRender::CreateDeviceD3D(hwnd))
    {
	    DxRender::CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }

    SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 0, LWA_COLORKEY);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    InitImGuiContext(hwnd, main_scale);

    bool done = false;
    while (!done) {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (DxRender::g_SwapChainOccluded && DxRender::g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        DxRender::g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (DxRender::g_ResizeWidth != 0 && DxRender::g_ResizeHeight != 0)
        {
	        DxRender::CleanupRenderTarget();
            DxRender::g_pSwapChain->ResizeBuffers(0, DxRender::g_ResizeWidth, DxRender::g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            DxRender::g_ResizeWidth = DxRender::g_ResizeHeight = 0;
            DxRender::CreateRenderTarget();
        }

        BeginImGuiFrame();

        RenderImGui();
        DxRender::Present();
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DxRender::CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return true;
}