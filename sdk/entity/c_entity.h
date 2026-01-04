#pragma once
#include "../../offsets.h"
#include "../../utils/memory.h"
#include "../../math/vec3.h"
#include "../client/base_client.h"
#include "../netvars/netvars.h"
#include <BaseTsd.h>
#include <unordered_map>
#include "../iclientshit/iclientnetworkable.h"
#include "../iclientshit/iclientrenderable.h"
#include "../iclientshit//iclientunknown.h"

#define MAX_BONES 128
#define BONE_PHYSICALLY_SIMULATED    0x01
#define BONE_PHYSICS_PROCEDURAL      0x02
#define BONE_ALWAYS_PROCEDURAL       0x04
#define BONE_SCREEN_ALIGN_SPHERE     0x08
#define BONE_SCREEN_ALIGN_CYLINDER   0x10

#define BONE_USED_MASK               0x0007FF00
#define BONE_USED_BY_ANYTHING        0x0007FF00

#define BONE_USED_BY_HITBOX          0x00000100
#define BONE_USED_BY_ATTACHMENT      0x00000200
#define BONE_USED_BY_VERTEX_LOD0     0x00000400
#define BONE_USED_BY_VERTEX_LOD1     0x00000800

enum CSWeaponClass
{
	WEAPON_KNIFE = 70,
	WEAPON_GLOCK = 173,
	WEAPON_G3SG1 = 171,
	WEAPON_AK47 = 1,
	WEAPON_AWP = 164,
	WEAPON_TMP = 184,
	WEAPON_DEAGLE = 31,
	WEAPON_M4A1 = 176,
	WEAPON_USP = 186,
	WEAPON_P228 = 179,
	WEAPON_SCOUT = 181,
	WEAPON_SG500 = 182,  // SG550
	WEAPON_SG552 = 183,
	WEAPON_AUG = 163,
	WEAPON_GALIL = 172,
	WEAPON_ELITE = 168,
	WEAPON_FIVESEVEN = 170,
	WEAPON_HEGRENADE = 65,
	WEAPON_FLASHBANG = 49,
	WEAPON_SMOKEGRENADE = 99,
	WEAPON_XM1014 = 187,
	WEAPON_M3 = 175,
	WEAPON_FAMAS = 169,
	WEAPON_M249 = 174,
	WEAPON_P90 = 180,
	WEAPON_MAC10 = 177,
	WEAPON_MP5N = 178,
	WEAPON_C4 = 23
};

enum BulletType
{
	BULLET_NONE = 0,
	BULLET_9MM,
	BULLET_45ACP,
	BULLET_50AE,
	BULLET_556MM,
	BULLET_762MM,
	BULLET_BUCKSHOT,
};

class IInterpolatedVar
{
public:
	virtual		 ~IInterpolatedVar() {}

	virtual void Setup(void* pValue, int type) = 0;
	virtual void SetInterpolationAmount(float seconds) = 0;

	// Returns true if the new value is different from the prior most recent value.
	virtual void NoteLastNetworkedValue() = 0;
	virtual bool NoteChanged(float changetime, bool bUpdateLastNetworkedValue) = 0;
	virtual void Reset() = 0;

	// Returns 1 if the value will always be the same if currentTime is always increasing.
	virtual int Interpolate(float currentTime) = 0;

	virtual int	 GetType() const = 0;
	virtual void RestoreToLastNetworked() = 0;
	virtual void Copy(IInterpolatedVar* pSrc) = 0;

	virtual const char* GetDebugName() = 0;
	virtual void SetDebugName(const char* pName) = 0;

	virtual void SetDebug(bool bDebug) = 0;
};


class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	std::vector< VarMapEntry_t >	m_Entries;
	int							m_nInterpolatedEntries;
	float						m_lastInterpolationTime;
};


struct WeaponStats {
	int   damage;
	float range;
	float rangeModifier;
	float penetration;
	float armorRatio;

	float spread;
	float inaccuracyCrouch;
	float inaccuracyStand;
	float inaccuracyJump;
	float inaccuracyMove;
	float inaccuracyFire;
	float inaccuracyLand;
	float inaccuracyLadder;

	int   bullets;
	float cycleTime;
};

inline bool IsGun(int id)
{
	if (id == WEAPON_AK47 || id == WEAPON_AWP || id == WEAPON_M4A1 ||
		id == WEAPON_FAMAS || id == WEAPON_AUG || id == WEAPON_GALIL ||
		id == WEAPON_SG552 || id == WEAPON_SCOUT || id == WEAPON_SG500 ||
		id == WEAPON_XM1014 || id == WEAPON_M3 || id == WEAPON_M249 ||
		id == WEAPON_P90 || id == WEAPON_MAC10 || id == WEAPON_MP5N ||
		id == WEAPON_TMP || id == WEAPON_G3SG1 ||

		id == WEAPON_GLOCK || id == WEAPON_USP || id == WEAPON_DEAGLE ||
		id == WEAPON_P228 || id == WEAPON_ELITE || id == WEAPON_FIVESEVEN)
		return true;

	return false;
}


inline bool IsRifle(int id)
{
	if (id == WEAPON_AK47 || id == WEAPON_M4A1 || id == WEAPON_AUG ||
		id == WEAPON_FAMAS || id == WEAPON_GALIL || id == WEAPON_SG552 ||
		id == WEAPON_SG500 || id == WEAPON_SCOUT || id == WEAPON_AWP)
		return true;
	return false;
}

inline bool IsSMG(int id)
{
	if (id == WEAPON_MP5N || id == WEAPON_MAC10 ||
		id == WEAPON_TMP || id == WEAPON_P90)
		return true;
	return false;
}

inline bool IsPistol(int id)
{
	if (id == WEAPON_GLOCK || id == WEAPON_USP || id == WEAPON_DEAGLE ||
		id == WEAPON_P228 || id == WEAPON_ELITE || id == WEAPON_FIVESEVEN)
		return true;
	return false;
}

inline bool IsGrenade(int id)
{
	if (id == WEAPON_HEGRENADE || id == WEAPON_FLASHBANG ||
		id == WEAPON_SMOKEGRENADE)
		return true;
	return false;
}

inline bool Sniper(int id)
{
	if (id == WEAPON_SCOUT || id == WEAPON_SG500 ||
		id == WEAPON_AWP)
		return true;
	return false;
}

struct ray_t;
struct trace_class;

class ICollideable
{
public:
	virtual void* GetEntityHandle() = 0;
	virtual const Vector3& OBBMinsPreScaled() const = 0;
	virtual const Vector3& OBBMaxsPreScaled() const = 0;
	virtual const Vector3& OBBMins() const = 0;
	virtual const Vector3& OBBMaxs() const = 0;
	virtual void  WorldSpaceTriggerBounds() const = 0;
	virtual bool  TestCollision(ray_t ray, unsigned int fContentsMask, trace_class tr) = 0;
};

enum HitboxGroup : int
{
	GROUP_STOMACH,
	GROUP_LEFT_LEG,
	GROUP_RIGHT_LEG,
	GROUP_CHEST,
	GROUP_NECK,
	GROUP_HEAD,
	GROUP_LEFT_ARM,
	GROUP_RIGHT_ARM
};

static std::unordered_map<HitboxGroup, std::vector<int>> Hitboxes =
{
	{ GROUP_STOMACH,   { 1, 5, 9, 10 } },
	{ GROUP_LEFT_LEG,  { 2, 3, 4 } },
	{ GROUP_RIGHT_LEG, { 6, 7, 8 } },
	{ GROUP_CHEST,     { 11, 12, 15 } },
	{ GROUP_NECK,      { 13 } },
	{ GROUP_HEAD,      { 14 } },
	{ GROUP_LEFT_ARM,  { 16, 17, 18 } },
	{ GROUP_RIGHT_ARM, { 29, 30, 31 } }
};

class CBaseWeapon;
class CViewModel;

class CAnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};

class BaseAnimating
{
public:

private:
};

class CEntity : public IClientEntity
{
private:
public:
	INETVAR("DT_BaseEntity", "m_vecNetworkOrigin", GetAbsOrigin, Vector3);
	INETVAR("DT_BaseEntity", "m_hConstraintEntity", ConstraintEntity, uint32_t);
	INETVAR("DT_BaseEntity", "m_angRotation", AngleRotation, Vector3);
	INETVAR("DT_BaseEntity", "m_nSequence", Sequence, int);
	INETVAR("DT_BaseEntity", "m_flCycle", Cycle, int);
	INETVAR("DT_BaseAnimating", "m_nHitboxSet", HitboxSet, int);
	INETVAR("DT_CSPlayer", "m_ArmorValue", ArmorValue, int);
	INETVAR("DT_CSPlayer", "m_bHasHelmet", HasHelmet, bool);
	INETVAR("DT_CSPlayer", "m_hRagdoll", isRagdoll, int);
	INETVAR("DT_Local", "m_bDrawViewmodel", drawViewModel, bool);

public:
	Vector3 EyePosition();
	Matrix3x4* BoneMatrix();
	Vector3 BonePosition(int bone);
	IClientNetworkable* Networkable();
	CClientClass* ClientClass();
	CBaseWeapon* GetWeapon();
	CAnimationLayer* AnimOverlay();
	bool SetupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

	/*void DumpEntityVTable(CEntity* ent)
	{
		void** vmt = *(void***)ent;
		printf("---- CEntity VMT DUMP ----\n");
		for (int i = 0; i < 30; i++)
		{
			printf("[%02d] 0x%llX\n", i, (unsigned long long)vmt[i]);
		}
	}*/
	const int MoveType() const noexcept
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x1F4);
	}
	const int& GetSkin() const noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0x987);
	}
	const int& GetBody() const noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0x9C7);
	}
	const int& Health() const noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + Offsets::m_iHealth);
	}
	Vector3& VecViewOffset() const noexcept
	{
		return *reinterpret_cast<Vector3*>(std::uintptr_t(this) + 0x23C);
	}
	Vector3& VecOrigin() const noexcept
	{
		return *reinterpret_cast<Vector3*>(std::uintptr_t(this) + 0x308);
	}
	const std::uint8_t& LifeState() const noexcept
	{
		return *reinterpret_cast<std::uint8_t*>(std::uintptr_t(this) + 0xCF);
	}
	const int& GetTeam() const noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0xD8);
	}
	const bool &Spotted() const noexcept
	{
		return *reinterpret_cast<bool*>(std::uintptr_t(this) + 0x1365);
	}
	bool Dormant() const noexcept
	{
		const auto addr = uintptr_t(this) + 0x1F8;
		if (!addr)
			return false;
		return *reinterpret_cast<int*>(addr) > 0;
	}
	Vector3& VecPunchAngle() noexcept
	{
		return *reinterpret_cast<Vector3*>(std::uintptr_t(this) + 0x127C);
	}
	Vector3& Velocity() noexcept
	{
		return *reinterpret_cast<Vector3*>(std::uintptr_t(this) + 0x1A8);
	}
	int& Flags() noexcept
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + 0x440);
	}
	inline int DrawModel(int flags, float alpha = 1.f) noexcept
	{
		return memory::GetVFunc<int(__fastcall*)(void*, int, float)>(this, 9)(this,flags, alpha);
	}
	inline int& UserId() noexcept
	{
		return *(int*)(uintptr_t(this) + 0x11AC);
	}
	inline float& FlashAlpha() noexcept
	{
		return *(float*)(uintptr_t(this) + 0x1A54);
	}
	inline float& NextAttack() noexcept
	{
		return *(float*)(uintptr_t(this) + 0x1058);
	}
	int ObserverTarget()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x15E0);
	}
	int GetWeaponHandle()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x11A0);
	}
	int& TickBase()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x16B0);
	}
	float flHeadHeight()
	{
		return *(float*)(uintptr_t(this) + 0x144);
	}
	ICollideable* GetCollideable()
	{
		return memory::CallVFunc<ICollideable*>(this, 3);
	}
	IClientRenderable* GetRenderable()
	{
		return memory::Call<IClientRenderable*>(this, 5);
	}
	inline float& SimulationTime()
	{
		return *reinterpret_cast<float*>((uintptr_t)this + 0xA8);
	}
	inline int& Index()
	{
		return *reinterpret_cast<int*>((uintptr_t)this + 0x88);
	}
	inline CUserCmd*& m_pCurrentCommand(CEntity* player)
	{
		return *reinterpret_cast<CUserCmd**>(reinterpret_cast<uintptr_t>(player) + +0x15B8);
	}
};

class CBaseWeapon : public CEntity
{
public:
	INETVAR("DT_BaseCombatWeapon", "m_iClip1", Ammo, int);
public:
	void GetWeaponStats(CSWeaponClass id, WeaponStats& out);

	float& NextPrimaryAttack()
	{
		return *reinterpret_cast<float*>((uintptr_t)this + 0xBE8);
	}
	float& NextSecondaryAttack()
	{
		return *reinterpret_cast<float*>((uintptr_t)this + 0xBEC);
	}
	float Inaccuracy()
	{
		return *reinterpret_cast<float*>((uintptr_t)this + 0xCB0);
	}
	float Spread()
	{
		using Fn = float(__fastcall*)(void*);
		return memory::GetVFunc<Fn>(this, 383)(this);
	}
	float SpreadCone()
	{
		return memory::GetVFunc<float(__fastcall*)(void*)>(this, 382)(this);
	}
	void UpdateAccuracy()
	{
		using Fn = void(__fastcall*)(void*);
		return memory::GetVFunc<Fn>(this, 384)(this);
	}
	int& ModelIndex()
	{
		return *reinterpret_cast<int*>((uintptr_t)this + 0xCC);
	}
};

class EntityList
{
public:
	constexpr CEntity* GetEntityFromIndex(const int index) noexcept
	{
		return memory::Call<CEntity*>(this, 3, index);
	}
	inline CEntity* GetClientEntityFromHandle(unsigned long handle)
	{
		int index = handle & 0xFFF;
		return GetEntityFromIndex(index);
	}
};

class CViewModel
{
public:
	CBaseWeapon* GetWeapon()
	{
		return reinterpret_cast<CBaseWeapon*>((uintptr_t)this + 0xC1C);
	}
	int& ModelIndex()
	{
		return *reinterpret_cast<int*>((uintptr_t)this + 0xCC);
	}
};