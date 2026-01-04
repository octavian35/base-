#pragma once
#include "../sdk.h"
#include "../../utils/memory.h"

class IMaterial;

class IVModelInfo
{
public:
	studiohdr_t* GetStudioHdr(const Model_t* model)
	{
		return memory::GetVFunc<studiohdr_t * (__fastcall*)(void*, const Model_t*)>(this, 27)(this, model);
	}
	int GetModelIndex(const char* name)
	{
		return memory::CallVFunc<int(__fastcall*)(void*, const char*)>(this, 3)(this, name);
	}
};

class IVModelRender
{
public:
	void ForceMaterial(IMaterial* mat, OverrideType_t type = OVERRIDE_NORMAL)
	{
		return memory::VCall<void(__fastcall*)(IVModelRender*, IMaterial*, OverrideType_t)>(this, 1)(this, mat, type);
	}
};