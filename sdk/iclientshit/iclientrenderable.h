#pragma once
#include "../../math/vec3.h"
#include "../../utils/memory.h"

class IClientUnknown;
struct Matrix3x4;

struct Model_t;

class IClientRenderable
{
public:
	constexpr IClientUnknown* GetIClientUnknown() noexcept
	{
		return memory::Call<IClientUnknown*>(this, 0);
	}
	Vector3 GetRenderOrigin() noexcept
	{
		return memory::CallVFunc<Vector3>(this, 1);
	}
	Vector3 GetRenderAngles() noexcept
	{
		return memory::CallVFunc<Vector3>(this, 2);
	}
	Matrix3x4* GetModelToWorld()
	{
		return memory::Call<Matrix3x4*>(this, 31);
	}
	Model_t* GetModel()
	{
		return memory::Call<Model_t*>(this, 10);
	}
};