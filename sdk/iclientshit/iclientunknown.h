#pragma once
#include "../../utils/memory.h"

class CEntity;
class IClientEntity;

class IClientUnknown
{
public:
	constexpr CEntity* GetBaseEntity() noexcept
	{
		return memory::Call<CEntity*>(this, 7);
	}
	constexpr IClientEntity* GetClientEntity() noexcept
	{
		return memory::Call<IClientEntity*>(this, 3);
	}
	constexpr IClientRenderable* GetRenderable() noexcept
	{
		return memory::Call<IClientRenderable*>(this, 5);
	}
};