#pragma once
#include "../utils/memory.h"

class VGuiPanel
{
public:
    const char* GetName(unsigned long long vguiPanel)
    {
        using OriginalFn = const char* (__thiscall*)(void*, unsigned long long);
        return memory::VCall<OriginalFn>(this, 36)(this, vguiPanel);
    }
private:
};