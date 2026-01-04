#pragma once
#include "../math/vec3.h"
#include "Windows.h"

struct studiomeshdata_t;
class Vector;
struct LightDesc_t;
struct studiohwdata_t;
struct Ray_t;
class Vector4D;
class IMesh;
struct vertexFileHeader_t;
struct FlashlightState_t;
class VMatrix;
namespace OptimizedModel { struct FileHeader_t; }
class IPooledVBAllocator;

struct mstudiobbox {
	int         bone;
	int         group;
	Vector3		bbmin;
	Vector3		bbmax;
	int         szhitboxnameindex;
	int         _pad00[8];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}
};

struct mstudiohitboxset {
	int                   sznameindex;
	inline char* const    pszName(void) const { return ((char*)this) + sznameindex; }
	int                   numhitboxes;
	int                   hitboxindex;
	inline mstudiobbox* pHitbox(int i) const { return (mstudiobbox*)(((unsigned char*)this) + hitboxindex) + i; };
};

struct studiohdr_t {
	unsigned char     _pad00[0xAC];
	int               numhitboxsets;
	int               hitboxsetindex;

	inline mstudiohitboxset* pHitboxSet(const int n) const {
		return (mstudiohitboxset*)(((unsigned char*)this) + hitboxsetindex) + n;
	};
	inline int GetHitboxCount(int set) const
	{
		mstudiohitboxset const* s = pHitboxSet(set);
		if (!s)
			return 0;
		return s->numhitboxes;
	}
};

struct StudioRenderConfig_t
{
	float fEyeShiftX;	// eye X position
	float fEyeShiftY;	// eye Y position
	float fEyeShiftZ;	// eye Z position
	float fEyeSize;		// adjustment to iris textures
	float fEyeGlintPixelWidthLODThreshold;

	int maxDecalsPerModel;
	int drawEntities;
	int skin;
	int fullbright;

	bool bEyeMove : 1;		// look around
	bool bSoftwareSkin : 1;
	bool bNoHardware : 1;
	bool bNoSoftware : 1;
	bool bTeeth : 1;
	bool bEyes : 1;
	bool bFlex : 1;
	bool bWireframe : 1;
	bool bDrawNormals : 1;
	bool bDrawTangentFrame : 1;
	bool bDrawZBufferedWireframe : 1;
	bool bSoftwareLighting : 1;
	bool bShowEnvCubemapOnly : 1;
	bool bWireframeDecals : 1;

	// Reserved for future use
	int m_nReserved[4];
};

enum
{
	ADDDECAL_TO_ALL_LODS = -1
};


//-----------------------------------------------------------------------------
// DrawModel flags
//-----------------------------------------------------------------------------
enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,

	STUDIORENDER_GENERATE_STATS = 0x8000,
};

#define VERTEX_TEXCOORD0_2D ( ( (uint64) 2 ) << ( TEX_COORD_SIZE_BIT + ( 3*0 ) ) )
/*
enum MaterialVertexFormat_t
{
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX,
	MATERIAL_VERTEX_FORMAT_MODEL = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D,
	MATERIAL_VERTEX_FORMAT_COLOR = (VertexFormat_t)VERTEX_SPECULAR
};
*/

//-----------------------------------------------------------------------------
// What kind of material override is it?
//-----------------------------------------------------------------------------
enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};


//-----------------------------------------------------------------------------
// DrawModel info
//-----------------------------------------------------------------------------

// Special flag for studio models that have a compiled in shadow lod version
// It's negative 2 since positive numbers == use a regular slot and -1 means
//  have studiorender compute a value instead
enum
{
	USESHADOWLOD = -2,
};

// beyond this number of materials, you won't get info back from DrawModel
#define MAX_DRAW_MODEL_INFO_MATERIALS 8

struct DrawModelResults_t
{
	int m_ActualTriCount;
	int m_TextureMemoryBytes;
	int m_NumHardwareBones;
	int m_NumBatches;
	int m_NumMaterials;
	int m_nLODUsed;
	int m_flLODMetric;
	void* m_RenderTime;
	void* m_Materials;
};

struct ColorTexelsInfo_t
{
	int						m_nWidth;
	int						m_nHeight;
	int						m_nMipmapCount;
	int						unused;
	int						m_nByteCount;
	byte* m_pTexelData;
};

struct ColorMeshInfo_t
{
	// A given color mesh can own a unique Mesh, or it can use a shared Mesh
	// (in which case it uses a sub-range defined by m_nVertOffset and m_nNumVerts)
	IMesh* m_pMesh;
	IPooledVBAllocator* m_pPooledVBAllocator;
	int						m_nVertOffsetInBytes;
	int						m_nNumVerts;
	void* m_pLightmap;
	ColorTexelsInfo_t* m_pLightmapData;
};

struct StudioDecalHandle_t;

struct DrawModelInfo_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	IClientRenderable* m_pRenderable;
	Matrix3x4* m_pModelToWorld;
	int						m_decals;
	int						m_drawFlags;
	int						m_lod;
};

enum ModType_t
{
    mod_bad = 0,
    mod_brush,
    mod_sprite,
    mod_studio
};

struct Model_t
{
    void*   fnHandle;       // 0x00 - IModelExtraData or internal handle
    char    szName[260];    // 0x08 - full model path e.g. "models/props/cs_office/..."
    __int32 nLoadFlags;     // 0x10C
    __int32 nServerCount;   // 0x110
    ModType_t type;         // 0x114 - <- THIS IS WHAT YOU NEED
    __int32 nFlags;         // 0x118

    // for studio models:
    void* pStudioHDR;       // 0x11C
    void* pStudioHWData;    // 0x120

    // for brush models:
    void* pBrushHeader;     // 0x124

    void* pad;              // 0x128
};

class IMaterial;

typedef unsigned short ModelInstanceHandle_t;

enum
{
	MODEL_INSTANCE_INVALID = (ModelInstanceHandle_t)~0
};

struct ModelRenderInfo_t
{
	Vector3 origin;
	Vector3 angles;
	IClientRenderable* pRenderable;
	const Model_t* pModel;
	const Matrix3x4* pModelToWorld;
	const Matrix3x4* pLightingOffset;
	const Vector3* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

//-----------------------------------------------------------------------------
// Cache Callback Function
// implementation can either statically persist data (tools) or lru cache (engine) it.
// caller returns base pointer to resident data.
// code expectes data to be dynamic and invokes cache callback prior to iterative access.
// virtualModel is member passed in via studiohdr_t and passed back for model identification.
//-----------------------------------------------------------------------------
#define STUDIO_DATA_CACHE_INTERFACE_VERSION "VStudioDataCache005"

class IStudioDataCache
{
public:
	virtual bool VerifyHeaders(studiohdr_t * pStudioHdr) = 0;
	virtual vertexFileHeader_t* CacheVertexData(studiohdr_t* pStudioHdr) = 0;
};

//-----------------------------------------------------------------------------
// Studio render interface
//-----------------------------------------------------------------------------
#define STUDIO_RENDER_INTERFACE_VERSION "VStudioRender025"

using StudioDrawModel_t = void(__fastcall*)(void* thisptr,
	DrawModelResults_t* results,
	const DrawModelInfo_t& info,
	Matrix3x4* boneToWorld,
	float* flexWeights,
	float* delayedFlex,
	const Vector& origin,
	int flags);

class IStudioRender
{
public:
	void SetColorModulation(const float* color) noexcept
	{
		memory::CallVFunc<void>(this, 27, color);
	}

	void SetAlphaModulation(const float alpha) noexcept
	{
		memory::CallVFunc<void>(this, 28, alpha);
	}

	void DrawModel(void* thisptr, DrawModelResults_t* results, const DrawModelInfo_t& info, Matrix3x4* boneToWorld, float* flexWeights, float* delayedFlex, const Vector3& origin, int flags)
	{
		memory::CallVFunc<StudioDrawModel_t>(this, 29);
	}

	void ForcedMaterialOverride(IMaterial* material, const std::int32_t type = OVERRIDE_NORMAL, const std::int32_t index = -1) noexcept
	{
		memory::CallVFunc<void>(this, 33, material, type, index);
	}
};

extern IStudioRender* StudioRender;