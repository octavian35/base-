#pragma once
#include "../math/vec3.h"
#include "../sdk/engine/c_move_helper.h"
#include "../sdk/entity/c_entity.h"

class IPrediction
{
public:
	virtual			~IPrediction(void) {};

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Run prediction
	virtual void
		Update
		(
			int startframe,				// World update ( un-modded ) most recently received
			bool validframe,			// Is frame data valid
			int incoming_acknowledged,	// Last command acknowledged to have been run by server (un-modded)
			int outgoing_command		// Last command (most recent) sent to server (un-modded)
		) = 0;

	// We are about to get a network update from the server.  We know the update #, so we can pull any
	//  data purely predicted on the client side and transfer it to the new from data state.
	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(Vector3& org) = 0;
	virtual void	SetViewOrigin(Vector3& org) = 0;
	virtual void	GetViewAngles(Vector3& ang) = 0;
	virtual void	SetViewAngles(Vector3& ang) = 0;
	virtual void	GetLocalViewAngles(Vector3& ang) = 0;
	virtual void	SetLocalViewAngles(Vector3& ang) = 0;
};


class CPrediction : public IPrediction
{
	// Construction
public:
	virtual			~CPrediction(void) = 0;

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Implement IPrediction
public:

	virtual void	Update
	(
		int startframe,		// World update ( un-modded ) most recently received
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual bool	InPrediction(void) const = 0;
	virtual bool	IsFirstTimePredicted(void) const = 0;

#if !defined( NO_ENTITY_PREDICTION )
	virtual int		GetIncomingPacketNumber(void) const = 0;
#endif

	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(Vector3& org) = 0;
	virtual void	SetViewOrigin(Vector3& org) = 0;
	virtual void	GetViewAngles(Vector3& ang) = 0;
	virtual void	SetViewAngles(Vector3& ang) = 0;

	virtual void	GetLocalViewAngles(Vector3& ang) = 0;
	virtual void	SetLocalViewAngles(Vector3& ang) = 0;

	virtual void	RunCommand(CEntity* player, CUserCmd* ucmd, CMoveHelper* moveHelper) = 0;

	// Internal
	virtual void	SetupMove(CEntity* player, CUserCmd* ucmd, CMoveHelper* pHelper, CMoveData* move) = 0;
	virtual void	FinishMove(CEntity* player, CUserCmd* ucmd, CMoveData* move) = 0;
	virtual void	SetIdealPitch(CEntity* player, const Vector3& origin, const Vector3& angles, const Vector3& viewheight) = 0;

	virtual void	_Update
	(
		bool received_new_world_update,
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;

public:
	// Last object the player was standing on
	CHandle< CEntity >		m_hLastGround;

	bool			m_bInPrediction;
	bool			m_bFirstTimePredicted;
	bool			m_bOldCLPredictValue;
	bool			m_bEnginePaused;
	int				m_nPreviousStartFrame;
	int				m_nCommandsPredicted;
	int				m_nServerCommandsAcknowledged;
	int				m_bPreviousAckHadErrors;
	int				m_nIncomingPacketNumber;
	float			m_flIdealPitch;
};