#pragma once
#include "../utils/memory.h"
#include "../sdk/entity/c_entity.h"
#include "../sdk/entity/c_handle.h"
#include "../sdk/entity/i_handle.h"

typedef CBaseHandle EntityHandle_t;

class CMoveHelper {
public:
	void set_host(CEntity* player) {
		typedef void(__thiscall* fn)(void*, CEntity*);
		return memory::GetVFunc<fn>(this, 12)(this, player);
	}
};

class CMoveData {
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;

	EntityHandle_t	m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	Vector3			m_vecViewAngles;	// Command view angles (local space)
	Vector3			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flOldForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector3			m_vecVelocity;		// edict::velocity		// Current movement direction.
	Vector3			m_vecAngles;		// edict::angles
	Vector3			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	Vector3			m_outWishVel;		// This is where you tried 
	Vector3			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	Vector3			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	Vector3			m_vecAbsOrigin;		// edict::origin
};

class CGameMovement
{
public:
	virtual			~CGameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(CEntity * pPlayer, CMoveData * pMove) = 0;
	virtual void	StartTrackPredictionErrors(CEntity* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CEntity* pPlayer) = 0;
	virtual void	Unused0() = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector3	GetPlayerMins(bool ducked) const = 0;
	virtual Vector3	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector3  GetPlayerViewOffset(bool ducked) const = 0;
};