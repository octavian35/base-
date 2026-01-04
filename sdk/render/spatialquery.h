#pragma once
#include "Windows.h"
#include "../math/vec3.h"

class IClientRenderable;
class CRenderableInfo
{
public:
	IClientRenderable* renderable;
	void* alphaProperty;
	std::int32_t enumCount;
	std::int32_t renderFrame;
	std::uint16_t firstShadow;
	std::uint16_t leafList;
	std::int16_t area;
	std::int16_t flags;
	std::int16_t flags2;
	Vector3 bloatedAbsMins;
	Vector3 bloatedAbsMaxs;
	Vector3 mins;
	Vector3 maxs;
	std::byte pad0[0x4];
};