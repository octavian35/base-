#pragma once
#include "../../math/vec3.h"
#include "recv_table.h"
#include "client_class.h"
#include "../const.h"
#include "../../utils/memory.h"
#include "../iclientshit/iclientnetworkable.h"
#include "../iclientshit/iclientrenderable.h"
#include "../iclientshit/iclientunknown.h"

enum FL {
	fl_on_ground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_water_jump = (1 << 2),
	fl_on_train = (1 << 3),
	fl_frozen = (1 << 5),
	fl_aicontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8)
};

enum player_controls
{
	in_attack = (1 << 0),
	in_jump = (1 << 1),
	in_duck = (1 << 2),
	in_forward = (1 << 3),
	in_back = (1 << 4),
	in_use = (1 << 5),
	in_cancel = (1 << 6),
	in_left = (1 << 7),
	in_right = (1 << 8),
	in_moveleft = (1 << 9),
	in_moveright = (1 << 10),
	in_attack2 = (1 << 11),
	in_run = (1 << 12),
	in_reload = (1 << 13),
	in_alt1 = (1 << 14),
	in_alt2 = (1 << 15),
	in_score = (1 << 16),
	in_speed = (1 << 17),
	in_walk = (1 << 18),
	in_zoom = (1 << 19),
	in_weapon1 = (1 << 20),
	in_weapon2 = (1 << 21),
	in_bullrush = (1 << 22)
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {}

	int     command_number;   // 0x04
	int     tick_count;       // 0x08
	Vector3 viewangles;       // 0x0C
	float   forwardmove;      // 0x18
	float   sidemove;         // 0x1C
	float   upmove;           // 0x20
	int     buttons;          // 0x24
	std::byte    impulse;          // 0x28
	int     weaponselect;     // 0x2C
	int     weaponsubtype;    // 0x30
	int     random_seed;      // 0x34
	short   mousedx;          // 0x38
	short   mousedy;          // 0x3A
	bool    hasbeenpredicted; // 0x3C
};

enum FrameStage_t {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class bf_read;

class CMouthInfo;
struct SpatializationInfo_t;

class IClientEntity : public IClientRenderable, public IClientNetworkable, public IClientUnknown
{
public:
	IClientNetworkable* GetClientNetworkable()
	{
		using Fn = IClientNetworkable * (__fastcall*)(void*);
		return ((Fn)(*(void***)(this))[0])(this);
	}

	CClientClass* GetClientClass()
	{
		auto net = GetClientNetworkable();
		if (!net)
			return nullptr;
		return net->GetClientClass();
	}
	public:
		virtual void Release(void) = 0;
		virtual const Vector3& GetAbsOrigin(void) const = 0;
		virtual const Vector3& GetAbsAngles(void) const = 0;
		virtual CMouthInfo* GetMouth(void) = 0;
		virtual bool GetSoundSpatialization(SpatializationInfo_t& info) = 0;

};