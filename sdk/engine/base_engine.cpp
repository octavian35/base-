#include "base_engine.h"
#include "../../interfaces/interfaces.h"
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "../../config/config.h"

bool VEngine::W2S(
    const Vector3& world,
    ImVec2& screen,
    const Vector3& eye,
    const Vector3& viewAngles,
    float fovX,
    float& scaleOut          // NEW OUTPUT
)
{
    Vector3 f, r, u;
    mathspace::AngleToBasis(viewAngles, f, r, u);

    Vector3 rel = world - eye;

    float cx = rel.dot(r);
    float cy = rel.dot(u);
    float cz = rel.dot(f);
    if (cz <= 0.0f)
        return false;

    constexpr float nearClamp = 0.01f;
    if (cz < nearClamp)
        cz = nearClamp;

    int width, height;
    Interfaces.IEngine->GetScreenSize(width, height);

    float aspect = float(width) / float(height);

    float fovY = 2.0f * atanf(tanf(mathspace::ToRadians(fovX) / 2.f) / aspect);
    float tanHalfY = tanf(fovY * 0.5f);

    float invDepth = 1.0f / cz;

    float x_ndc = -(cx * invDepth) / (tanHalfY * aspect);
    float y_ndc = (cy * invDepth) / tanHalfY;

    if (!std::isfinite(x_ndc) || !std::isfinite(y_ndc))
        return false;

    screen.x = (width * 0.5f) + (x_ndc * width * 0.5f);
    screen.y = (height * 0.5f) - (y_ndc * height * 0.5f);

    // Apply forced aspect ratio
    if (check.aspect_ratio && check.aspect_value > 0.f)
    {
        float nativeAspect = float(width) / float(height);
        float forcedAspect = check.aspect_value;
        float factor = nativeAspect / forcedAspect;

        screen.x = (screen.x - width * 0.5f) * factor + width * 0.5f;
    }

    float dist = rel.length();

    scaleOut = 1.0f - (dist / 6000.0f);
    scaleOut = std::clamp(scaleOut, 0.55f, 1.0f);

    return true;
}

bool VEngine::ScreenTransform(const Vector3& point, ImVec2& screen) // tots not pasted
{
    float w;
    const Matrix4x4& worldToScreen = Interfaces.IEngine->WorldToScreenMatrix();

    screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
    screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
    w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];

    bool behind = false;

    if (w < 0.001f)
    {
        behind = true;
        screen.x *= 100000;
        screen.y *= 100000;
    }
    else
    {
        behind = false;
        float invw = 1.0f / w;
        screen.x *= invw;
        screen.y *= invw;
    }

    return behind;
}

bool VEngine::WorldToScreen(const Vector3& origin, ImVec2& screen)
{
    if (!ScreenTransform(origin, screen))
    {
        int w, h;
        GetScreenSize(w, h);

        float x = w / 2;
        float y = h / 2;
        x += 0.5 * screen.x * w + 0.5;
        y -= 0.5 * screen.y * h + 0.5;
        screen.x = x;
        screen.y = y;
        return true;
    }

    return false;
}