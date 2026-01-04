#include "hooks.h"
#include <stdexcept>
#include <intrin.h>
#include <thread>
#include <algorithm>

#include "../sdk/sdk.h"
#include "../minhook/MinHook.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include <iostream>
#include <variant>

#undef min
#undef max

using CreateMoveFN = bool(__fastcall*)(void* thisptr, float frametime, CUserCmd* cmd);
using DrawModelFN = void(__fastcall*)(void* edx, const DrawModelInfo_t& pState, const ModelRenderInfo_t& pInfo, Matrix3x4* pBoneToWorld);
using FrameStageFN = void(__fastcall*)(void* rdx, int mode);
using RenderFN = void(__fastcall*)(void* that, CViewSetup& view, int nClearFlags, int whatToDraw);
using TraceRayFN = void(__fastcall*)(void* thisptr, const ray_t* rayptr, unsigned int mask, void* filterptr, void* outptr);
using RunCommandFN = void(__fastcall*)(void* ecx, CEntity* player, CUserCmd* ucmd, CMoveHelper* MoveHelper);
using PaintFN = void(__fastcall*)(void* rcx, int mode);
using StudioDrawModelFN = void(__fastcall*)(void* thisptr,
    DrawModelResults_t* results,
    const DrawModelInfo_t& info,
    Matrix3x4* boneToWorld,
    float* flexWeights,
    float* delayedFlex,
    const Vector3& origin,
    int flags);

using CLMoveFN = void(__cdecl*)(float, bool);
inline CLMoveFN CLMove = nullptr;

CreateMoveFN CreateMove = {};
DrawModelFN DrawModel = {};
FrameStageFN FrameStage = {};
StudioDrawModelFN StudioDraw = {};
RenderFN Render = {};
TraceRayFN TraceRay = {};
RunCommandFN RunCommand = {};
PaintFN Paint = {};

void __fastcall TraceRayHook(void* thisptr, const ray_t* rayptr, unsigned int mask, void* filterptr, void* outptr)
{
    return TraceRay(thisptr, rayptr, mask, filterptr, outptr);
}

void __fastcall RunCommandHook(void* ecx, CEntity* player, CUserCmd* ucmd, CMoveHelper* MoveHelper)
{
    Interfaces.MoveHelper = MoveHelper;

    return RunCommand(ecx, player, ucmd, MoveHelper);
}


void __cdecl CL_MoveHook(float accumulated_extra_samples, bool final_tick)
{
    return CLMove(accumulated_extra_samples, final_tick);
}


void RenderHook(void* that, CViewSetup& view, int nClearFlags, int whatToDraw)
{
    return Render(that, view, nClearFlags, whatToDraw);
}

void __fastcall PaintHook(void* ecx, void* edx, int mode)
{
    return Paint(ecx, edx, mode);
}

void __fastcall FrameStageHook(void* rcx, int stage) noexcept
{
    return FrameStage(rcx, stage);
}

bool __fastcall CreateMoveHook(void* thisptr, float frametime, CUserCmd* cmd) noexcept
{
    if (!cmd || !cmd->command_number)
        return CreateMove(thisptr, frametime, cmd);

    return false;
}

void __fastcall DrawModelHook(void* edx, const DrawModelInfo_t& pState, const ModelRenderInfo_t& pInfo, Matrix3x4* pBoneToWorld) noexcept
{
    return DrawModel(edx, pState, pInfo, pBoneToWorld);
}

void hooks::Setup()
{
    if (MH_Initialize())
        throw std::runtime_error("failed to init minhook!");

    MH_CreateHook(VF(Menu::device, 42), &EndScene, reinterpret_cast<void**>(&EndSceneOrg));

    MH_CreateHook(VF(Menu::device, 16), &Reset, reinterpret_cast<void**>(&ResetOrg));

    auto get_virtual = [](void* _class, unsigned int index)
        {
            return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
        };

    auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
    auto offset = 0x68CFE8;

    auto pointer = client + offset;

    uintptr_t clientmode = *reinterpret_cast<uintptr_t*>(pointer);

    uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(clientmode);

    auto target = reinterpret_cast<void*>(vtable[21]);

    MH_CreateHook(target, CreateMoveHook, reinterpret_cast<void**>(&CreateMove));

    MH_CreateHook(reinterpret_cast<void*>(VF(Interfaces.ModelRender, 19)), DrawModelHook, reinterpret_cast<void**>(&DrawModel));

    MH_CreateHook(reinterpret_cast<void*>(VF(Interfaces.IClient, 35)), FrameStageHook, reinterpret_cast<void**>(&FrameStage));

    MH_CreateHook(reinterpret_cast<void*>(VF(Interfaces.Trace, 4)), TraceRayHook, reinterpret_cast<void**>(&TraceRay));

    MH_CreateHook(reinterpret_cast<void*>(VF(Interfaces.Prediction, 17)), RunCommandHook, reinterpret_cast<void**>(&RunCommand));

    //MH_CreateHook(reinterpret_cast<void*>(VF(Interfaces.EngineVGui, 14)), PaintHook, reinterpret_cast<void**>(&Paint));

    MH_CreateHook(Pattern.render_view, RenderHook, reinterpret_cast<void**>(&Render));

    MH_CreateHook(Pattern.cl_move_pattern, CL_MoveHook, reinterpret_cast<void**>(&CLMove));

    MH_EnableHook(MH_ALL_HOOKS);

    Menu::DestroyDX();
}

void hooks::Destroy() noexcept
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
    const auto result = EndSceneOrg(device, device);

    if (!Menu::setup)
        Menu::SetupMenu(device);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = ResetOrg(device, device, params);
    ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}