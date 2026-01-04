#include "c_entity.h"
#include "../../interfaces/interfaces.h"
#include "../../pattern/pattern.h"
#include "../../calls.h"

Vector3 CEntity::EyePosition()
{
	Vector3 VecView = VecOrigin();
	VecView.z += flHeadHeight();
	return VecView;
}

Matrix3x4* CEntity::BoneMatrix()
{
	return *reinterpret_cast<Matrix3x4**>(reinterpret_cast<uintptr_t>(this) + 0x810);
}

Vector3 CEntity::BonePosition(int bone)
{
	Matrix3x4* matrix = BoneMatrix();
	if (!matrix || bone <= 0 || !g.IsReadableAddress(matrix))
		return Vector3(0, 0, 0);
	
	return Vector3
	{
		matrix[bone][0][3],
		matrix[bone][1][3],
		matrix[bone][2][3]
	};
}

IClientNetworkable* CEntity::Networkable()
{
	return reinterpret_cast<IClientNetworkable*>(uintptr_t(this) + 0x10);
}

CClientClass* CEntity::ClientClass()
{
	using original_fn = CClientClass * (__fastcall*)(void*);
	return (*(original_fn**)Networkable())[2](Networkable());
}

CBaseWeapon* CEntity::GetWeapon()
{
	int Handle = GetWeaponHandle();
	return reinterpret_cast<CBaseWeapon*>(Interfaces.IEntityList->GetClientEntityFromHandle(Handle));
}

CAnimationLayer* CEntity::AnimOverlay()
{
	return reinterpret_cast<CAnimationLayer*>(uintptr_t(this) + 0x19F8);
}

void CBaseWeapon::GetWeaponStats(CSWeaponClass id, WeaponStats& out)
{
    switch (id)
    {
    case WEAPON_M3:
        out = { 26,3000.f,0.70f,1.f,1.00f,
                0.04000f,0.00750f,0.01000f,0.42000f,
                0.04320f,0.04164f,0.08400f,0.07875f,
                9,0.88f };
        break;

    case WEAPON_XM1014:
        out = { 22,3000.f,0.70f,1.f,1.00f,
                0.035f,0.008f,0.012f,0.420f,
                0.040f,0.040f,0.080f,0.080f,
                7,0.30f };
        break;

    case WEAPON_DEAGLE:
        out = { 54,4096.f,0.81f,2.f,1.50f,
                0.00400f,0.00975f,0.01300f,0.34500f,
                0.02070f,0.05500f,0.06900f,0.02300f,
                1,0.225f };
        break;

    case WEAPON_ELITE:
        out = { 45,4096.f,0.75f,1.f,1.00f,
                0.00400f,0.00600f,0.00800f,0.29625f,
                0.01778f,0.03162f,0.05925f,0.01975f,
                1,0.12f };
        break;

    case WEAPON_FIVESEVEN:
        out = { 25,4096.f,0.885f,1.f,1.30f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                1,0.15f };
        break;

    case WEAPON_GLOCK:
        out = { 25,4096.f,0.75f,1.f,1.00f,
                0.00400f,0.00750f,0.01000f,0.27750f,
                0.01665f,0.03167f,0.05550f,0.01850f,
                1,0.15f };
        break;

    case WEAPON_USP:
        out = { 34,4096.f,0.79f,1.f,1.40f,
                0.00400f,0.00600f,0.00800f,0.28725f,
                0.01724f,0.03495f,0.05745f,0.01915f,
                1,0.15f };
        break;

    case WEAPON_P228:
        out = { 40,4096.f,0.80f,1.f,1.30f,
                0.00400f,0.00825f,0.01100f,0.28500f,
                0.01710f,0.03318f,0.05700f,0.01900f,
                1,0.15f };
        break;

    case WEAPON_MAC10:
        out = { 29,4096.f,0.82f,1.f,1.00f,
                0.00100f,0.01425f,0.01900f,0.13704f,
                0.00620f,0.00845f,0.02741f,0.03426f,
                1,0.075f };
        break;

    case WEAPON_MP5N:
        out = { 26,4096.f,0.84f,1.f,1.00f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                1,0.08f };
        break;

    case WEAPON_P90:
        out = { 26,4096.f,0.84f,1.f,1.00f,
                0.00100f,0.01463f,0.01951f,0.16494f,
                0.01062f,0.00732f,0.03299f,0.04124f,
                1,0.07f };
        break;

    case WEAPON_TMP:
        out = { 26,4096.f,0.84f,1.f,1.00f,
                0.00100f,0.01500f,0.02000f,0.11180f,
                0.00389f,0.01594f,0.02236f,0.02795f,
                1,0.07f };
        break;

    case WEAPON_M4A1:
        out = { 33,8192.f,0.97f,2.f,1.40f,
                0.00060f,0.00525f,0.00700f,0.34151f,
                0.06872f,0.01266f,0.06830f,0.08538f,
                1,0.09f };
        break;

    case WEAPON_AK47:
        out = { 36,8192.f,0.98f,2.f,1.55f,
                0.00060f,0.00687f,0.00916f,0.43044f,
                0.09222f,0.01158f,0.08609f,0.10761f,
                1,0.10f };
        break;

    case WEAPON_AUG:
        out = { 32,8192.f,0.96f,2.f,1.40f,
                0.f,0.f,0.f,0.f,
                0.f,0.f,0.f,0.f,
                1,0.09f };
        break;

    case WEAPON_SG552:
        out = { 33,8192.f,0.955f,2.f,1.40f,
                0.00060f,0.00405f,0.00540f,0.33464f,
                0.06132f,0.01227f,0.06693f,0.08366f,
                1,0.09f };
        break;

    case WEAPON_SCOUT:
        out = { 75,8192.f,0.98f,3.f,1.90f,
                0.00030f,0.02378f,0.03170f,0.38195f,
                0.19097f,0.06667f,0.03819f,0.09549f,
                1,1.25f };
        break;

    case WEAPON_SG500: // SG550
        out = { 70,8192.f,0.98f,2.f,1.80f,
                0.00030f,0.01928f,0.02570f,0.43727f,
                0.21864f,0.03829f,0.04373f,0.10932f,
                1,0.25f };
        break;

    case WEAPON_G3SG1:
        out = { 80,8192.f,0.98f,3.f,2.00f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                0.00001f,0.00001f,0.00001f,0.00001f,
                1,0.25f };
        break;

    case WEAPON_AWP:
        out = { 115,8192.f,0.99f,3.f,2.00f,
                0.f,0.f,0.f,0.f,
                0.f,0.f,0.f,0.f,
                1,1.45f };
        break;

    case WEAPON_M249:
        out = { 32,8192.f,0.97f,2.f,1.30f,
                0.0012f,0.015f,0.020f,0.38f,
                0.08f,0.06f,0.07f,0.05f,
                1,0.075f };
        break;

    default:
        out = {};
        break;
    }
}

bool CEntity::SetupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	return memory::GetVFunc<bool(__fastcall*)(void*, Matrix3x4*, int, int, float)>(this, 16)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}