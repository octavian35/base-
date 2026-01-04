#pragma once
#include "../math/vec3.h"

class IVDebugOverlay
{
public:
	bool ScreenPosition(const Vector3& in, Vector3& out);

};