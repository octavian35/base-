#include <stdexcept>
#include <windows.h>
#include "hooks/hooks.h"
#include <iostream>

HMODULE hlmodule = nullptr;
bool loop = true;

void injection_thread()
{
	try
	{
		Interfaces.initialize();
		cfg.initialize();
		hooks::Setup();
	}
	catch (const std::exception& err)
	{
		// nothing bruh
	}
	while (loop)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			loop = false;
	}

	hooks::Destroy();
	FreeLibraryAndExitThread(hlmodule, 1);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	
	switch (reason)
	{
	case DLL_PROCESS_ATTACH: {
		hlmodule = module;
		Window::base = (uintptr_t)GetModuleHandleW(nullptr);
		DisableThreadLibraryCalls(module);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(injection_thread), nullptr, 0, nullptr);
		} break;
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}