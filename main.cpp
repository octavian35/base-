#include <thread>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <stdio.h>

#include "Core/globals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "config/config.h"
#include "sdk/entity/c_entity.h"
#include "interfaces/interfaces.h"
#include "../sdk/sdk.h"

// Forward declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

bool Menu::setup_wnd_class(const char* class_name) noexcept
{
    wnd_class.cbSize = sizeof(WNDCLASSEX);
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc = DefWindowProc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = GetModuleHandle(NULL);
    wnd_class.hIcon = 0;
    wnd_class.hCursor = 0;
    wnd_class.hbrBackground = 0;
    wnd_class.lpszMenuName = 0;
    wnd_class.lpszClassName = class_name;
    wnd_class.hIconSm = 0;
    return RegisterClassEx(&wnd_class);
}

void Menu::destroy_wnd_class() noexcept { UnregisterClass(wnd_class.lpszClassName, wnd_class.hInstance); }

bool Menu::setup_hwnd(const char* name) noexcept
{
    hwnd = CreateWindow(wnd_class.lpszClassName, name, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, wnd_class.hInstance, 0);
    return (hwnd != NULL);
}

void Menu::destroy_hwnd() noexcept { if (hwnd) DestroyWindow(hwnd); }

bool Menu::SetupDX() noexcept
{
    const auto handle = GetModuleHandle("d3d9.dll");
    if (!handle) return false;

    using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
    const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));
    if (!create) return false;

    d3d9 = create(D3D_SDK_VERSION);
    if (!d3d9) return false;

    D3DPRESENT_PARAMETERS params = { 0 };
    params.Windowed = 1;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = hwnd;

    if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &device) < 0)
        return false;

    return true;
}

void Menu::DestroyDX() noexcept
{
    if (device) { device->Release(); device = NULL; }
    if (d3d9) { d3d9->Release(); d3d9 = NULL; }
}

void Menu::Core()
{
    if (!setup_wnd_class("class1")) throw std::runtime_error("Failed class");
    if (!setup_hwnd("BaseMenu")) throw std::exception("Failed hwnd");
    if (!SetupDX()) throw std::runtime_error("Failed device");
    destroy_hwnd();
    destroy_wnd_class();
}

void Menu::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
    D3DDEVICE_CREATION_PARAMETERS params{};
    device->GetCreationParameters(&params);
    hwnd = params.hFocusWindow;
    org_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(device);
    
    // Default Style
    ImGui::StyleColorsDark();
    setup = true;
}

void Menu::Destroy() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(org_wndproc));
    DestroyDX();
}

void Menu::Render() noexcept
{
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("Cheat Base", nullptr, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }

    ImGui::End();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_KEYDOWN && wparam == VK_INSERT)
        Menu::show_overlay = !Menu::show_overlay;

    if (Menu::show_overlay && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

    if (Menu::show_overlay && g_ListeningBind)
    {
        if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
        {
            if (wparam == VK_ESCAPE) {
                g_ListeningBind = nullptr;
                return true;
            }
            g_ListeningBind->key = static_cast<int>(wparam);
            g_ListeningBind = nullptr;
            return true;
        }
        else if (msg == WM_LBUTTONDOWN) { g_ListeningBind->key = VK_LBUTTON; g_ListeningBind = nullptr; return true; }
        else if (msg == WM_RBUTTONDOWN) { g_ListeningBind->key = VK_RBUTTON; g_ListeningBind = nullptr; return true; }
        else if (msg == WM_MBUTTONDOWN) { g_ListeningBind->key = VK_MBUTTON; g_ListeningBind = nullptr; return true; }
        else if (msg == WM_XBUTTONDOWN) { 
            g_ListeningBind->key = (HIWORD(wparam) == XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2; 
            g_ListeningBind = nullptr; 
            return true; 
        }
    }

    return CallWindowProc(Menu::org_wndproc, hwnd, msg, wparam, lparam);
}