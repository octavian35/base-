#include "debug_overlay.h"
#include "../../utils/memory.h"

bool IVDebugOverlay::ScreenPosition(const Vector3& in, Vector3& out)
{
	return memory::CallVFunc<bool>(this, 13, in, out);
}