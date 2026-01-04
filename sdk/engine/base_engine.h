#pragma once
#include "../../offsets.h"
#include "../../math/math.h"
#include "../../math/vec3.h"
#include "../../utils/memory.h"
#include "Windows.h"
#include "../entity/c_entity.h"
#include "../render/imaterial.h"
#include <string>
#include <imgui.h>
#include <d3dx9math.h>
#include <d3d9.h>
#include "inetchannelinfo.h"

#define SIGNED_GUID_LEN 32
#define MAX_PLAYER_NAME_LENGTH 32

typedef unsigned int CRC32_t;

#pragma pack(push, 1)
struct player_info_t {
	char name[32];
	int userID;
	char guid[33];
	unsigned int friendsID;
	char friendsName[32];
	bool fakeplayer;
	bool ishltv;
	bool isreplay;
	unsigned char pad;
	unsigned int customFiles[4];
	unsigned char filesDownloaded;
	unsigned char pad2[3];
};
#pragma pack(pop)

enum w2s_method {
	custom,
	in_game
};

class VEngine
{
public:
	bool WorldToScreen(const Vector3& origin, ImVec2& screen);
	bool ScreenTransform(const Vector3& origin, ImVec2& screen);
	bool W2S(const Vector3& world,
		ImVec2& screen,
		const Vector3& eye,
		const Vector3& viewAngles,
		float fovX,
		float& scaleOut);
    void GetViewAngles(Vector3& angles)
    {
        typedef void(__fastcall* Fn)(__int64, DWORD*);
        DWORD temp[3]; // allocate temp DWORD array
        ((Fn)memory::VCall<Fn>(this, 19))((__int64)this, temp);

        // convert DWORD to float
        angles.x = *(float*)&temp[0];
        angles.y = *(float*)&temp[1];
        angles.z = *(float*)&temp[2];
    }
    void SetViewAngles(Vector3& angles)
    {
        typedef void(__fastcall* Fn)(void*, Vector3&);
        ((Fn)memory::VCall<Fn>(this, 20))(this, angles);
    }
    int GetLocalPlayer()
    {
        using Fn = int(__fastcall*)(void*);
        return ((Fn)memory::VCall<Fn>(this, 12))(this);
    }
	void GetScreenSize(std::int32_t& width, std::int32_t& height) noexcept
	{
		memory::CallVFunc<void>(this, 5, std::ref(width), std::ref(height));
	}
	Matrix4x4& WorldToScreenMatrix() noexcept
	{
		return memory::CallVFunc<Matrix4x4&>(this, 36);
	}
	void unrestrictedCmd(const char* szCmdString, bool cmd = false)
	{
		return memory::CallVFunc<void>(this, 102, szCmdString, cmd);
	}    
	bool GetPlayerInfo(int ent_num, player_info_t* pinfo)
	{
		return memory::CallVFunc<bool>(this, 8, ent_num, pinfo);
	}
	bool GetPlayerName(int index, std::string& outName)
	{
		player_info_t info{};
		if (GetPlayerInfo(index, &info))
		{
			outName = info.name;
			return true;
		}
		return false;
	}
	float GetTime()
	{
		return memory::CallVFunc<float>(this, 14);
	}
	INetChannelInfo* GetNetChannelInfo()
	{
		using Fn = INetChannelInfo * (__fastcall*)(void*);
		return memory::GetVFunc<Fn>(this, 72)(this);
	}
private:
};

#pragma pack(push,1)
struct vec4f { float x, y, z, w; }; // 16 bytes

struct ray_t
{
	vec4f start;        // 0x00..0x0F  (engine loads as __m128)
	vec4f delta;        // 0x10..0x1F
	vec4f startOffset;  // 0x20..0x2F
	vec4f extents;      // 0x30..0x3F
	// Flags live at +0x40..0x4F; engine reads the whole 16B, and checks a byte at +0x40
	uint8_t isRay;      // 0x40
	uint8_t isSwept;    // 0x41
	uint8_t _padFlags[14]; // 0x42..0x4F
	// total size = 0x50 (80) — matches the 5 x 16B blocks your IDA shows being read
	void Init(const Vector3& start_, const Vector3& end_)
	{
		start = { start_.x, start_.y, start_.z, 0.0f };
		Vector3 d = end_ - start_;
		delta = { d.x, d.y, d.z, 0.0f };

		// "pure ray" (no hull)
		startOffset = { 0.f, 0.f, 0.f, 0.f };
		extents = { 0.f, 0.f, 0.f, 0.f };

		isSwept = (d.x != 0.0f || d.y != 0.0f || d.z != 0.0f) ? 1 : 0;
		isRay = 1; // CRITICAL: the decomp checks a byte at +0x40; make sure this is 1
		memset(_padFlags, 0, sizeof(_padFlags));
	}
};
#pragma pack(pop)
static_assert(sizeof(ray_t) == 0x50, "ray_t must be 0x50 bytes");

struct trace_filter {
	trace_filter(const CEntity* entity) : skip{ entity } {}
	virtual bool shouldHitEntity(CEntity* entity, int) { return entity != skip; }
	virtual int getTraceType() const { return 0; }
	const void* skip;
};

struct csurface_t
{
	const char *name;
	short       surfaceProps;
	unsigned short flags;
};

struct cplane_t
{
	Vector3	normal;
	float	dist;
	byte	type;			// for fast side tests
	byte	signbits;		// signx + (signy<<1) + (signz<<1)
	byte	pad[2];

#ifdef VECTOR_NO_SLOW_OPERATIONS
	cplane_t() {}

private:
	// No copy constructors allowed if we're in optimal mode
	cplane_t(const cplane_t& vOther);
#endif
};

struct trace_class {
	Vector3			startpos;				// start position
	Vector3			endpos;					// final position
	cplane_t		cplane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	float		fractionleftsolid;		// time we left a solid, only valid if we started in solid
	csurface_t	surface;				// surface hit (impact surface)

	int			hitgroup;				// 0 == generic, non-zero is specific body part
	short		physicsbone;			// physics bone hit by trace in studio

	CEntity* entity;					// entity the surface is part of

	int			hitbox;					// box hit by trace in studio
};

class IHandleEntity;

class trace_ray {
public:
	int GetPointContents(const Vector3& absOrigin, IHandleEntity** ppEntity = nullptr)
	{
		return memory::CallVFunc<int>(this, 0, absOrigin, ppEntity);
	}

	void get_trace_ray(const ray_t* ray, unsigned int mask, trace_filter* filter, trace_class* out)
	{
		using original_fn = void(__fastcall*)(void* thisptr, const ray_t* rayptr, unsigned int mask, void* filterptr, void* outptr);

		// vtable index 4 per your earlier wrapper — keep that if you verified it
		auto vtbl = *reinterpret_cast<void***>(this);
		auto fn = reinterpret_cast<original_fn>(vtbl[4]);

		return fn(this, ray, mask, reinterpret_cast<void*>(filter), reinterpret_cast<void*>(out));
	}
};

enum EVGuiPanel
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};

class CViewSetup
{
public:
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	// which eye are we rendering?
	StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vector3		origin;

	// heading of camera (pitch, yaw, roll)
	Vector3		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_bViewToProjectionOverride;
	Matrix4x4     m_ViewToProjection;
};

class RenderView
{
public:
	virtual void			DrawBrushModel(IClientEntity* baseentity, int* model, const Vector3& origin, const Vector3& angles, bool bUnused) = 0;
	virtual void			DrawIdentityBrushModel(void* pList, int* model) = 0;
	virtual void			TouchLight(struct dlight_t* light) = 0;
	virtual void			Draw3DDebugOverlays(void) = 0;
	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;
	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;
	virtual void			SceneBegin(void) = 0;
	virtual void			SceneEnd(void) = 0;
	virtual void			GetVisibleFogVolume(const Vector3& eyePoint, void* pInfo) = 0;
	virtual void* CreateWorldList() = 0;
	virtual void			BuildWorldLists(void* pList, void* pInfo, int iForceFViewLeaf, const void* pVisData = NULL, bool bShadowDepth = false, float* pReflectionWaterHeight = NULL) = 0;
	virtual void			DrawWorldLists(void* pList, unsigned long flags, float waterZAdjust) = 0;
	virtual void			DrawTopView(bool enable) = 0;
	virtual void			TopViewBounds(Vector2 const& mins, Vector2 const& maxs) = 0;
	virtual void			DrawLights(void) = 0;
	virtual void			DrawMaskEntities(void) = 0;
	virtual void			DrawTranslucentSurfaces(void* pList, int sortIndex, unsigned long flags, bool bShadowDepth) = 0;
	virtual void			DrawLineFile(void) = 0;
	virtual void			DrawLightmaps(void* pList, int pageId) = 0;
	virtual void			ViewSetupVis(bool novis, int numorigins, const Vector3 origin[]) = 0;
	virtual bool			AreAnyLeavesVisible(int* leafList, int nLeaves) = 0;
	virtual	void			VguiPaint(void) = 0;
	virtual void			ViewDrawFade(byte* color, IMaterial* pMaterial) = 0;
	virtual void			OLD_SetProjectionMatrix(float fov, float zNear, float zFar) = 0;
	virtual Vector3		GetLightAtPoint(Vector3& pos) = 0;
	virtual int				GetViewEntity(void) = 0;
	virtual float			GetFieldOfView(void) = 0;
	virtual unsigned char** GetAreaBits(void) = 0;
	virtual void			SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;
	virtual void			InstallBrushSurfaceRenderer(void* pBrushRenderer) = 0;
	virtual void			DrawBrushModelShadow(IClientRenderable* pRenderable) = 0;
	virtual	bool			LeafContainsTranslucentSurfaces(void* pList, int sortIndex, unsigned long flags) = 0;
	virtual bool			DoesBoxIntersectWaterVolume(const Vector3& mins, const Vector3& maxs, int leafWaterDataID) = 0;
	virtual void			Unused1() = 0;
	virtual void			VGui_Paint(int mode) = 0;
	virtual void			Unused2() = 0;
	virtual void			Unused3() = 0;
	virtual void			Unused4() = 0;
	virtual void			SetMainView(const Vector3& vecOrigin, const Vector3& angles) = 0;
	enum
	{
		VIEW_SETUP_VIS_EX_RETURN_FLAGS_USES_RADIAL_VIS = 0x00000001
	};
	virtual void			ViewSetupVisEx(bool novis, int numorigins, const Vector3 origin[], unsigned int& returnFlags) = 0;
	virtual void			Unused5() = 0;
	virtual void			Unused6() = 0;
	virtual void			Unused7() = 0;
	virtual void			BeginUpdateLightmaps(void) = 0;
	virtual void			EndUpdateLightmaps(void) = 0;
	virtual void			OLD_SetOffCenterProjectionMatrix(float fov, float zNear, float zFar, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight) = 0;
	virtual void			OLD_SetProjectionMatrixOrtho(float left, float top, float right, float bottom, float zNear, float zFar) = 0;
	virtual void			Unused8() = 0;
	virtual void			GetMatricesForView(const CViewSetup& view, Matrix4x4* pWorldToView, Matrix4x4* pViewToProjection, Matrix4x4* pWorldToProjection, Matrix4x4* pWorldToPixels) = 0;
	virtual void			Unused9() = 0;
	
	void SetColorModulation(float r, float g, float b)
	{
		float blend[3] = { r, g, b };
		SetColorModulation(blend);
	}
};


class IEngineVGui
{
public:
	constexpr std::uintptr_t GetPanel(std::int32_t type) noexcept
	{
		return memory::Call<std::uintptr_t>(this, 1, type);
	}
};