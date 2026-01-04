//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#ifndef IMATERIAL_H
#define IMATERIAL_H

#ifdef _WIN32
#pragma once
#endif

#include "imaterialvar.h"

//-----------------------------------------------------------------------------
// forward declaraions
//-----------------------------------------------------------------------------

class IMaterialVar;
class ITexture;
class IMaterialProxy;
class Vector;

//-----------------------------------------------------------------------------
// Flags for GetVertexFormat
//-----------------------------------------------------------------------------
#define	VERTEX_POSITION					0x0001
#define	VERTEX_NORMAL					0x0002
#define	VERTEX_COLOR					0x0004
#define	VERTEX_SPECULAR					0x0008

#define	VERTEX_TANGENT_S				0x0010
#define	VERTEX_TANGENT_T				0x0020
#define	VERTEX_TANGENT_SPACE			( VERTEX_TANGENT_S | VERTEX_TANGENT_T )

// Indicates we're using wrinkle
#define	VERTEX_WRINKLE					0x0040

// Indicates we're using bone indices
#define	VERTEX_BONE_INDEX				0x0080

// Indicates this is a vertex shader
#define	VERTEX_FORMAT_VERTEX_SHADER		0x0100

// Indicates this format shouldn't be bloated to cache align it
// (only used for VertexUsage)
#define	VERTEX_FORMAT_USE_EXACT_FORMAT	0x0200

// Indicates that compressed vertex elements are to be used (see also VertexCompressionType_t)
#define	VERTEX_FORMAT_COMPRESSED		0x400

// Update this if you add or remove bits...
#define	VERTEX_LAST_BIT					10

#define	VERTEX_BONE_WEIGHT_BIT			(VERTEX_LAST_BIT + 1)
#define	USER_DATA_SIZE_BIT				(VERTEX_LAST_BIT + 4)
#define	TEX_COORD_SIZE_BIT				(VERTEX_LAST_BIT + 7)

#define	VERTEX_BONE_WEIGHT_MASK			( 0x7 << VERTEX_BONE_WEIGHT_BIT )
#define	USER_DATA_SIZE_MASK				( 0x7 << USER_DATA_SIZE_BIT )

#define	VERTEX_FORMAT_FIELD_MASK		0x0FF

// If everything is off, it's an unknown vertex format
#define	VERTEX_FORMAT_UNKNOWN			0



//-----------------------------------------------------------------------------
// Macros for construction..
//-----------------------------------------------------------------------------
#define VERTEX_BONEWEIGHT( _n )				((_n) << VERTEX_BONE_WEIGHT_BIT)
#define VERTEX_USERDATA_SIZE( _n )			((_n) << USER_DATA_SIZE_BIT)

//-----------------------------------------------------------------------------
// VertexElement_t (enumerates all usable vertex elements)
//-----------------------------------------------------------------------------
// FIXME: unify this with VertexFormat_t (i.e. construct the lower bits of VertexFormat_t with "1 << (VertexElement_t)element")
enum VertexElement_t
{
	VERTEX_ELEMENT_NONE = -1,

	// Deliberately explicitly numbered so it's a pain to change, so you read this:
	// #!#!#NOTE#!#!# update GetVertexElementSize, VertexElementToDeclType and
	//                CVBAllocTracker (elementTable) when you update this!
	VERTEX_ELEMENT_POSITION = 0,
	VERTEX_ELEMENT_NORMAL = 1,
	VERTEX_ELEMENT_COLOR = 2,
	VERTEX_ELEMENT_SPECULAR = 3,
	VERTEX_ELEMENT_TANGENT_S = 4,
	VERTEX_ELEMENT_TANGENT_T = 5,
	VERTEX_ELEMENT_WRINKLE = 6,
	VERTEX_ELEMENT_BONEINDEX = 7,
	VERTEX_ELEMENT_BONEWEIGHTS1 = 8,
	VERTEX_ELEMENT_BONEWEIGHTS2 = 9,
	VERTEX_ELEMENT_BONEWEIGHTS3 = 10,
	VERTEX_ELEMENT_BONEWEIGHTS4 = 11,
	VERTEX_ELEMENT_USERDATA1 = 12,
	VERTEX_ELEMENT_USERDATA2 = 13,
	VERTEX_ELEMENT_USERDATA3 = 14,
	VERTEX_ELEMENT_USERDATA4 = 15,
	VERTEX_ELEMENT_TEXCOORD1D_0 = 16,
	VERTEX_ELEMENT_TEXCOORD1D_1 = 17,
	VERTEX_ELEMENT_TEXCOORD1D_2 = 18,
	VERTEX_ELEMENT_TEXCOORD1D_3 = 19,
	VERTEX_ELEMENT_TEXCOORD1D_4 = 20,
	VERTEX_ELEMENT_TEXCOORD1D_5 = 21,
	VERTEX_ELEMENT_TEXCOORD1D_6 = 22,
	VERTEX_ELEMENT_TEXCOORD1D_7 = 23,
	VERTEX_ELEMENT_TEXCOORD2D_0 = 24,
	VERTEX_ELEMENT_TEXCOORD2D_1 = 25,
	VERTEX_ELEMENT_TEXCOORD2D_2 = 26,
	VERTEX_ELEMENT_TEXCOORD2D_3 = 27,
	VERTEX_ELEMENT_TEXCOORD2D_4 = 28,
	VERTEX_ELEMENT_TEXCOORD2D_5 = 29,
	VERTEX_ELEMENT_TEXCOORD2D_6 = 30,
	VERTEX_ELEMENT_TEXCOORD2D_7 = 31,
	VERTEX_ELEMENT_TEXCOORD3D_0 = 32,
	VERTEX_ELEMENT_TEXCOORD3D_1 = 33,
	VERTEX_ELEMENT_TEXCOORD3D_2 = 34,
	VERTEX_ELEMENT_TEXCOORD3D_3 = 35,
	VERTEX_ELEMENT_TEXCOORD3D_4 = 36,
	VERTEX_ELEMENT_TEXCOORD3D_5 = 37,
	VERTEX_ELEMENT_TEXCOORD3D_6 = 38,
	VERTEX_ELEMENT_TEXCOORD3D_7 = 39,
	VERTEX_ELEMENT_TEXCOORD4D_0 = 40,
	VERTEX_ELEMENT_TEXCOORD4D_1 = 41,
	VERTEX_ELEMENT_TEXCOORD4D_2 = 42,
	VERTEX_ELEMENT_TEXCOORD4D_3 = 43,
	VERTEX_ELEMENT_TEXCOORD4D_4 = 44,
	VERTEX_ELEMENT_TEXCOORD4D_5 = 45,
	VERTEX_ELEMENT_TEXCOORD4D_6 = 46,
	VERTEX_ELEMENT_TEXCOORD4D_7 = 47,

	VERTEX_ELEMENT_NUMELEMENTS = 48
};

// We're testing 2 normal compression methods
// One compressed normals+tangents into a SHORT2 each (8 bytes total)
// The other compresses them together, into a single UBYTE4 (4 bytes total)
// FIXME: pick one or the other, compare lighting quality in important cases
#define COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2	0
#define COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4	1
//#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2
#define COMPRESSED_NORMALS_TYPE						COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4

//-----------------------------------------------------------------------------
// Shader state flags can be read from the FLAGS materialvar
// Also can be read or written to with the Set/GetMaterialVarFlags() call
// Also make sure you add/remove a string associated with each flag below to CShaderSystem::ShaderStateString in ShaderSystem.cpp
//-----------------------------------------------------------------------------
enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in imaterialinternal.h
};


//-----------------------------------------------------------------------------
// Internal flags not accessible from outside the material system. Stored in Flags2
//-----------------------------------------------------------------------------
enum MaterialVarFlags2_t
{
	// NOTE: These are for $flags2!!!!!
//	UNUSED											= (1 << 0),

	MATERIAL_VAR2_LIGHTING_UNLIT = 0,
	MATERIAL_VAR2_LIGHTING_VERTEX_LIT = (1 << 1),
	MATERIAL_VAR2_LIGHTING_LIGHTMAP = (1 << 2),
	MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP = (1 << 3),
	MATERIAL_VAR2_LIGHTING_MASK =
	(MATERIAL_VAR2_LIGHTING_VERTEX_LIT |
		MATERIAL_VAR2_LIGHTING_LIGHTMAP |
		MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP),

	// FIXME: Should this be a part of the above lighting enums?
	MATERIAL_VAR2_DIFFUSE_BUMPMAPPED_MODEL = (1 << 4),
	MATERIAL_VAR2_USES_ENV_CUBEMAP = (1 << 5),
	MATERIAL_VAR2_NEEDS_TANGENT_SPACES = (1 << 6),
	MATERIAL_VAR2_NEEDS_SOFTWARE_LIGHTING = (1 << 7),
	// GR - HDR path puts lightmap alpha in separate texture...
	MATERIAL_VAR2_BLEND_WITH_LIGHTMAP_ALPHA = (1 << 8),
	MATERIAL_VAR2_NEEDS_BAKED_LIGHTING_SNAPSHOTS = (1 << 9),
	MATERIAL_VAR2_USE_FLASHLIGHT = (1 << 10),
	MATERIAL_VAR2_USE_FIXED_FUNCTION_BAKED_LIGHTING = (1 << 11),
	MATERIAL_VAR2_NEEDS_FIXED_FUNCTION_FLASHLIGHT = (1 << 12),
	MATERIAL_VAR2_USE_EDITOR = (1 << 13),
	MATERIAL_VAR2_NEEDS_POWER_OF_TWO_FRAME_BUFFER_TEXTURE = (1 << 14),
	MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE = (1 << 15),
	MATERIAL_VAR2_IS_SPRITECARD = (1 << 16),
	MATERIAL_VAR2_USES_VERTEXID = (1 << 17),
	MATERIAL_VAR2_SUPPORTS_HW_SKINNING = (1 << 18),
	MATERIAL_VAR2_SUPPORTS_FLASHLIGHT = (1 << 19),
};


//-----------------------------------------------------------------------------
// Preview image return values
//-----------------------------------------------------------------------------
enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE,
};


//-----------------------------------------------------------------------------
// material interface
//-----------------------------------------------------------------------------
class IMaterial
{
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual PreviewImageRetVal_t GetPreviewImageProperties(int* width, int* height,
		int* imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char* data,
												 int width, int height,
												 int imageFormat) const = 0;
	// 
	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;

	virtual int				GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float* pOffset) = 0;
	virtual void			GetMaterialScale(float* pScale) = 0;
	virtual IMaterial* GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int 			GetEnumerationID(void) const = 0;

	virtual void			GetLowResColorSample(float s, float t, float* color) const = 0;

	// This computes the state snapshots for this material
	virtual void			RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool			IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool			IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool			IsVertexLit() = 0;

	// Gets the vertex format
	virtual void	GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool			HasProxy(void) const = 0;

	virtual bool			UsesEnvCubemap(void) = 0;

	virtual bool			NeedsTangentSpace(void) = 0;

	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool			NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void			SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

	// Gets material reflectivity
	virtual void			GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool			GetPropertyFlag(int type) = 0;

	// Is the material visible from both sides?
	virtual bool			IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void			SetShader(const char* pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void			Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool			NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int				ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool			IsErrorMaterial() const = 0;

	virtual void			SetUseFixedFunctionBakedLighting(bool bEnable) = 0;

	// Gets the current alpha modulation
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float* r, float* g, float* b) = 0;

	// Gets the morph format
	virtual void	GetMorphFormat() const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void			SetShaderAndParams(void* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;

	virtual void			DeleteIfUnreferenced() = 0;

	virtual bool			IsSpriteCard() = 0;

	virtual void			CallBindProxy(void* proxyData) = 0;

	virtual IMaterial* CheckProxyReplacement(void* proxyData) = 0;

	virtual void			RefreshPreservingMaterialVars() = 0;

	virtual bool			WasReloadedFromWhitelist() = 0;

	virtual bool			IsPrecached() const = 0;
};


inline bool IsErrorMaterial(IMaterial* pMat)
{
	return !pMat || pMat->IsErrorMaterial();
}

#endif

class IMaterialSystem
{
public:

	// Placeholder for API revision
	virtual bool Connect(int factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	//---------------------------------------------------------
	// Initialization and shutdown
	//---------------------------------------------------------

	// Call this to initialize the material system
	// returns a method to create interfaces in the shader dll
	virtual void	Init(char const* pShaderAPIDLL,
		void* pMaterialProxyFactory,
		int fileSystemFactory,
		int cvarFactory = NULL) = 0;

	// Call this to set an explicit shader version to use 
	// Must be called before Init().
	virtual void				SetShaderAPI(char const* pShaderAPIDLL) = 0;

	// Must be called before Init(), if you're going to call it at all...
	virtual void				SetAdapter(int nAdapter, int nFlags) = 0;

	// Call this when the mod has been set up, which may occur after init
	// At this point, the game + gamebin paths have been set up
	virtual void				ModInit() = 0;
	virtual void				ModShutdown() = 0;

	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	virtual void					SetThreadMode(int mode, int nServiceThread = -1) = 0;
	virtual int						GetThreadMode() = 0;
	virtual bool					IsRenderThreadSafe() = 0;
	virtual void					ExecuteQueued() = 0;

	//---------------------------------------------------------
	// Config management
	//---------------------------------------------------------

	virtual void* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;


	// Call this before rendering each frame with the current config
	// for the material system.
	// Will do whatever is necessary to get the material system into the correct state
	// upon configuration change. .doesn't much else otherwise.
	virtual bool				UpdateConfig(bool bForceUpdate) = 0;

	// Force this to be the config; update all material system convars to match the state
	// return true if lightmaps need to be redownloaded
	virtual bool				OverrideConfig(const int& config, bool bForceUpdate) = 0;

	// Get the current config for this video card (as last set by UpdateConfig)
	virtual const int& GetCurrentConfigForVideoCard() const = 0;

	// Gets *recommended* configuration information associated with the display card, 
	// given a particular dx level to run under. 
	// Use dxlevel 0 to use the recommended dx level.
	// The function returns false if an invalid dxlevel was specified

	// UNDONE: To find out all convars affected by configuration, we'll need to change
	// the dxsupport.pl program to output all column headers into a single keyvalue block
	// and then we would read that in, and send it back to the client
	virtual bool				GetRecommendedConfigurationInfo(int nDXLevel, int* pKeyValues) = 0;


	// -----------------------------------------------------------
	// Device methods
	// -----------------------------------------------------------

	// Gets the number of adapters...
	virtual int					GetDisplayAdapterCount() const = 0;

	// Returns the current adapter in use
	virtual int					GetCurrentAdapter() const = 0;

	// Returns info about each adapter
	virtual void				GetDisplayAdapterInfo(int adapter, int& info) const = 0;

	// Returns the number of modes
	virtual int					GetModeCount(int adapter) const = 0;

	// Returns mode information..
	virtual void				GetModeInfo(int adapter, int mode, int& info) const = 0;

	virtual void				AddModeChangeCallBack(int func) = 0;

	// Returns the mode info for the current display device
	virtual void				GetDisplayMode(int& mode) const = 0;

	// Sets the mode...
	virtual bool				SetMode(void* hwnd, const int& config) = 0;

	virtual bool				SupportsMSAAMode(int nMSAAMode) = 0;

	// FIXME: REMOVE! Get video card identitier
	virtual const int& GetVideoCardIdentifier(void) const = 0;

	// Use this to spew information about the 3D layer 
	virtual void				SpewDriverInfo() const = 0;

	virtual void				GetDXLevelDefaults(int& max_dxlevel, int& recommended_dxlevel) = 0;

	// Get the image format of the back buffer. . useful when creating render targets, etc.
	virtual void				GetBackBufferDimensions(int& width, int& height) const = 0;
	virtual int			GetBackBufferFormat() const = 0;

	virtual bool				SupportsHDRMode(int nHDRModede) = 0;


	// -----------------------------------------------------------
	// Window methods
	// -----------------------------------------------------------

	// Creates/ destroys a child window
	virtual bool				AddView(void* hwnd) = 0;
	virtual void				RemoveView(void* hwnd) = 0;

	// Sets the view
	virtual void				SetView(void* hwnd) = 0;


	// -----------------------------------------------------------
	// Control flow
	// -----------------------------------------------------------

	virtual void				BeginFrame(float frameTime) = 0;
	virtual void				EndFrame() = 0;
	virtual void				Flush(bool flushHardware = false) = 0;

	/// FIXME: This stuff needs to be cleaned up and abstracted.
	// Stuff that gets exported to the launcher through the engine
	virtual void				SwapBuffers() = 0;

	// Flushes managed textures from the texture cacher
	virtual void				EvictManagedResources() = 0;

	virtual void				ReleaseResources(void) = 0;
	virtual void				ReacquireResources(void) = 0;


	// -----------------------------------------------------------
	// Device loss/restore
	// -----------------------------------------------------------

	// Installs a function to be called when we need to release vertex buffers + textures
	virtual void				AddReleaseFunc(int func) = 0;
	virtual void				RemoveReleaseFunc(int func) = 0;

	// Installs a function to be called when we need to restore vertex buffers
	virtual void				AddRestoreFunc(int func) = 0;
	virtual void				RemoveRestoreFunc(int func) = 0;

	// Release temporary HW memory...
	virtual void				ResetTempHWMemory(bool bExitingLevel = false) = 0;

	// For dealing with device lost in cases where SwapBuffers isn't called all the time (Hammer)
	virtual void				HandleDeviceLost() = 0;


	// -----------------------------------------------------------
	// Shaders
	// -----------------------------------------------------------

	// Used to iterate over all shaders for editing purposes
	// GetShaders returns the number of shaders it actually found
	virtual int					ShaderCount() const = 0;
	virtual int					GetShaders(int nFirstShader, int nMaxCount, void** ppShaderList) const = 0;

	// FIXME: Is there a better way of doing this?
	// Returns shader flag names for editors to be able to edit them
	virtual int					ShaderFlagCount() const = 0;
	virtual const char* ShaderFlagName(int nIndex) const = 0;

	// Gets the actual shader fallback for a particular shader
	virtual void				GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;


	// -----------------------------------------------------------
	// Material proxies
	// -----------------------------------------------------------

	virtual void* GetMaterialProxyFactory() = 0;

	// Sets the material proxy factory. Calling this causes all materials to be uncached.
	virtual void				SetMaterialProxyFactory(void* pFactory) = 0;


	// -----------------------------------------------------------
	// Editor mode
	// -----------------------------------------------------------

	// Used to enable editor materials. Must be called before Init.
	virtual void				EnableEditorMaterials() = 0;


	// -----------------------------------------------------------
	// Stub mode mode
	// -----------------------------------------------------------

	// Force it to ignore Draw calls.
	virtual void				SetInStubMode(bool bInStubMode) = 0;


	//---------------------------------------------------------
	// Debug support
	//---------------------------------------------------------

	virtual void				DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;
	virtual void				DebugPrintUsedTextures(void) = 0;

	virtual void				ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void				ToggleDebugMaterial(char const* pMaterialName) = 0;


	//---------------------------------------------------------
	// Misc features
	//---------------------------------------------------------
	//returns whether fast clipping is being used or not - needed to be exposed for better per-object clip behavior
	virtual bool				UsingFastClipping(void) = 0;

	virtual int					StencilBufferBits(void) = 0; //number of bits per pixel in the stencil buffer


	//---------------------------------------------------------
	// Material and texture management
	//---------------------------------------------------------

	// uncache all materials. .  good for forcing reload of materials.
	virtual void				UncacheAllMaterials() = 0;

	// Remove any materials from memory that aren't in use as determined
	// by the IMaterial's reference count.
	virtual void				UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;

	// Load any materials into memory that are to be used as determined
	// by the IMaterial's reference count.
	virtual void				CacheUsedMaterials() = 0;

	// Force all textures to be reloaded from disk.
	virtual void				ReloadTextures() = 0;

	// Reloads materials
	virtual void				ReloadMaterials(const char* pSubString = NULL) = 0;

	// Create a procedural material. The keyvalues looks like a VMT file
	virtual IMaterial* CreateMaterial(const char* pMaterialName, void* pVMTKeyValues) = 0;

	// Find a material by name.
	// The name of a material is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	// eg. "dev/dev_bumptest" refers to somethign similar to:
	// "d:/hl2/hl2/materials/dev/dev_bumptest.vmt"
	//
	// Most of the texture groups for pTextureGroupName are listed in texture_group_names.h.
	// 
	// Note: if the material can't be found, this returns a checkerboard material. You can 
	// find out if you have that material by calling IMaterial::IsErrorMaterial().
	// (Or use the global IsErrorMaterial function, which checks if it's null too).
	virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;

	// Query whether a material is loaded (eg, whether FindMaterial will be nonblocking)
	virtual bool				IsMaterialLoaded(char const* pMaterialName) = 0;

	//---------------------------------
	// This is the interface for knowing what materials are available
	// is to use the following functions to get a list of materials.  The
	// material names will have the full path to the material, and that is the 
	// only way that the directory structure of the materials will be seen through this
	// interface.
	// NOTE:  This is mostly for worldcraft to get a list of materials to put
	// in the "texture" browser.in Worldcraft
	virtual int	FirstMaterial() const = 0;

	// returns InvalidMaterial if there isn't another material.
	// WARNING: you must call GetNextMaterial until it returns NULL, 
	// otherwise there will be a memory leak.
	virtual int	NextMaterial(int h) const = 0;

	// This is the invalid material
	virtual int	InvalidMaterial() const = 0;

	// Returns a particular material
	virtual IMaterial* GetMaterial(int h) const = 0;

	// Get the total number of materials in the system.  These aren't just the used
	// materials, but the complete collection.
	virtual int					GetNumMaterials() const = 0;

	//---------------------------------

	virtual void				SetAsyncTextureLoadCache(void* hFileCache) = 0;

	virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;

	// Checks to see if a particular texture is loaded
	virtual bool				IsTextureLoaded(char const* pTextureName) const = 0;

	// Creates a procedural texture
	virtual ITexture* CreateProceduralTexture(const char* pTextureName,
		const char* pTextureGroupName,
		int w,
		int h,
		int fmt,
		int nFlags) = 0;

	//
	// Render targets
	//
	virtual void				BeginRenderTargetAllocation() = 0;
	virtual void				EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources

	// Creates a render target
	// If depth == true, a depth buffer is also allocated. If not, then
	// the screen's depth buffer is used.
	// Creates a texture for use as a render target
	virtual ITexture* unused0() = 0;

	virtual ITexture* unused1() = 0;

	virtual ITexture* unused2() = 0;

	// Must be called between the above Begin-End calls!
	virtual ITexture* unused3() = 0;

	// -----------------------------------------------------------
	// Lightmaps
	// -----------------------------------------------------------

	// To allocate lightmaps, sort the whole world by material twice.
	// The first time through, call AllocateLightmap for every surface.
	// that has a lightmap.
	// The second time through, call AllocateWhiteLightmap for every 
	// surface that expects to use shaders that expect lightmaps.
	virtual void				BeginLightmapAllocation() = 0;
	virtual void				EndLightmapAllocation() = 0;

	// returns the sorting id for this surface
	virtual int 				AllocateLightmap(int width, int height,
		int offsetIntoLightmapPage[2],
		IMaterial* pMaterial) = 0;
	// returns the sorting id for this surface
	virtual int					AllocateWhiteLightmap(IMaterial* pMaterial) = 0;

	// lightmaps are in linear color space
	// lightmapPageID is returned by GetLightmapPageIDForSortID
	// lightmapSize and offsetIntoLightmapPage are returned by AllocateLightmap.
	// You should never call UpdateLightmap for a lightmap allocated through
	// AllocateWhiteLightmap.
	virtual void				UpdateLightmap(int lightmapPageID, int lightmapSize[2],
		int offsetIntoLightmapPage[2],
		float* pFloatImage, float* pFloatImageBump1,
		float* pFloatImageBump2, float* pFloatImageBump3) = 0;

	// fixme: could just be an array of ints for lightmapPageIDs since the material
	// for a surface is already known.
	virtual int					GetNumSortIDs() = 0;
	virtual void				GetSortInfo(void* sortInfoArray) = 0;

	// Read the page size of an existing lightmap by sort id (returned from AllocateLightmap())
	virtual void				GetLightmapPageSize(int lightmap, int* width, int* height) const = 0;

	virtual void				ResetMaterialLightmapPageInfo() = 0;



	virtual void				ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	virtual void* GetRenderContext() = 0;

	virtual bool				SupportsShadowDepthTextures(void) = 0;
	virtual void				BeginUpdateLightmaps(void) = 0;
	virtual void				EndUpdateLightmaps(void) = 0;

	// -----------------------------------------------------------
	// Methods to force the material system into non-threaded, non-queued mode
	// -----------------------------------------------------------
	virtual int		Lock() = 0;
	virtual void				Unlock(int) = 0;

	// Vendor-dependent shadow depth texture format
	virtual int			GetShadowDepthTextureFormat() = 0;

	virtual bool				SupportsFetch4(void) = 0;

	// Create a custom render context. Cannot be used to create MATERIAL_HARDWARE_CONTEXT
	virtual void* CreateRenderContext(int type) = 0;

	// Set a specified render context to be the global context for the thread. Returns the prior context.
	virtual void* SetRenderContext(void*) = 0;

	virtual bool				SupportsCSAAMode(int nNumSamples, int nQualityLevel) = 0;

	virtual void				RemoveModeChangeCallBack(int func) = 0;

	// Finds or create a procedural material.
	virtual IMaterial* FindProceduralMaterial(const char* pMaterialName, const char* pTextureGroupName, void* pVMTKeyValues) = 0;

	virtual int			GetNullTextureFormat() = 0;

	virtual void				AddTextureAlias(const char* pAlias, const char* pRealName) = 0;
	virtual void				RemoveTextureAlias(const char* pAlias) = 0;

	// returns a lightmap page ID for this allocation, -1 if none available
	// frameID is a number that should be changed every frame to prevent locking any textures that are
	// being used to draw in the previous frame
	virtual int					AllocateDynamicLightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;

	virtual void				SetExcludedTextures(const char* pScriptName) = 0;
	virtual void				UpdateExcludedTextures(void) = 0;

	virtual bool				IsInFrame() const = 0;

	virtual void				CompactMemory() = 0;

	// For sv_pure mode. The filesystem figures out which files the client needs to reload to be "pure" ala the server's preferences.
	virtual void ReloadFilesInList(void* pFilesToReload) = 0;
	virtual	bool				AllowThreading(bool bAllow, int nServiceThread) = 0;

	// Extended version of FindMaterial().
	// Contains context in so it can make decisions (i.e. if it's a model, ignore certain cheat parameters)
	virtual IMaterial* FindMaterialEx(char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain = true, const char* pComplainPrefix = NULL) = 0;

#ifdef DX_TO_GL_ABSTRACTION
	virtual void				DoStartupShaderPreloading(void) = 0;
#endif	

	// Sets the override sizes for all render target size tests. These replace the frame buffer size.
	// Set them when you are rendering primarily to something larger than the frame buffer (as in VR mode).
	virtual void				SetRenderTargetFrameBufferSizeOverrides(int nWidth, int nHeight) = 0;

	// Returns the (possibly overridden) framebuffer size for render target sizing.
	virtual void				GetRenderTargetFrameBufferDimensions(int& nWidth, int& nHeight) = 0;

	// returns the display device name that matches the adapter index we were started with
	virtual char* GetDisplayDeviceName() const = 0;

	// creates a texture suitable for use with materials from a raw stream of bits.
	// The bits will be retained by the material system and can be freed upon return.
	virtual ITexture* CreateTextureFromBits(int w, int h, int mips, int fmt, int srcBufferSize, byte* srcBits) = 0;

	// Lie to the material system to pretend to be in render target allocation mode at the beginning of time.
	// This was a thing that mattered a lot to old hardware, but doesn't matter at all to new hardware,
	// where new is defined to be "anything from the last decade." However, we want to preserve legacy behavior
	// for the old games because it's easier than testing them.
	virtual void				OverrideRenderTargetAllocation(bool rtAlloc) = 0;

	// creates a texture compositor that will attempt to composite a new textuer from the steps of the specified KeyValues.
	virtual int* NewTextureCompositor(int w, int h, const char* pCompositeName, int nTeamNum, __int64 randomSeed, int* stageDesc, int texCompositeCreateFlags = 0) = 0;

	// Loads the texture with the specified name, calls pRecipient->OnAsyncFindComplete with the result from the main thread.
	// once the texture load is complete. If the texture cannot be found, the returned texture will return true for IsError().
	virtual void AsyncFindTexture(const char* pFilename, const char* pTextureGroupName, int* pRecipient, void* pExtraArgs, bool bComplain = true, int nAdditionalCreationFlags = 0) = 0;

	// creates a texture suitable for use with materials from a raw stream of bits.
	// The bits will be retained by the material system and can be freed upon return.
	virtual ITexture* CreateNamedTextureFromBitsEx(const char* pName, const char* pTextureGroupName, int w, int h, int mips, int fmt, int srcBufferSize, byte* srcBits, int nFlags) = 0;

	// Creates a texture compositor template for use in later code. 
	virtual bool				AddTextureCompositorTemplate(const char* pName, void* pTmplDesc, int nTexCompositeTemplateFlags = 0) = 0;

	// Performs final verification of all compositor templates (after they've all been initially loaded).
	virtual bool				VerifyTextureCompositorTemplates() = 0;

	virtual int		GetRenderBackend() const = 0;

	// Stop attempting to stream in textures in response to usage.  Useful for phases such as loading or other explicit
	// operations that shouldn't take usage of textures as a signal to stream them in at full rez.
	virtual void				SuspendTextureStreaming() = 0;
	virtual void				ResumeTextureStreaming() = 0;
};
