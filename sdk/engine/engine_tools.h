#pragma once
#include "../../math/vec3.h"

class IEngineTool
{
public:
	virtual void		GetServerFactory() = 0;
	virtual void		GetClientFactory() = 0;

	virtual float		GetSoundDuration(const char* pszName) = 0;
	virtual bool		IsSoundStillPlaying(int guid) = 0;
	// Returns the guid of the sound
	virtual int			StartSound();

	virtual void		StopSoundByGuid(int guid) = 0;

	// Returns how long the sound is
	virtual float		GetSoundDuration(int guid) = 0;

	// Returns if the sound is looping
	virtual bool		IsLoopingSound(int guid) = 0;
	virtual void		ReloadSound(const char* pSample) = 0;
	virtual void		StopAllSounds() = 0;
	virtual float		GetMono16Samples() = 0;
	virtual void		SetAudioState() = 0;

	// Issue a console command
	virtual void		Command(char const* cmd) = 0;
	// Flush console command buffer right away
	virtual void		Execute() = 0;

	virtual char const* GetCurrentMap() = 0;
	virtual void		ChangeToMap(char const* mapname) = 0;
	virtual bool		IsMapValid(char const* mapname) = 0;

	// Method for causing engine to call client to render scene with no view model or overlays
	// See cdll_int.h for enum RenderViewInfo_t for specifying whatToRender
	virtual void		RenderView() = 0;

	// Returns true if the player is fully connected and active in game (i.e, not still loading)
	virtual bool		IsInGame() = 0;
	// Returns true if the player is connected, but not necessarily active in game (could still be loading)
	virtual bool		IsConnected() = 0;

	virtual int			GetMaxClients() = 0; // Tools might want to ensure single player, e.g.

	virtual bool		IsGamePaused() = 0;
	virtual void		SetGamePaused(bool paused) = 0;

	virtual float		GetTimescale() = 0; // Could do this via ConVar system, too
	virtual void		SetTimescale(float scale) = 0;

	// Real time is unscaled, but is updated once per frame
	virtual float		GetRealTime() = 0;
	virtual float		GetRealFrameTime() = 0; // unscaled

	// Get high precision timer (for profiling?)
	virtual float		Time() = 0;

	// Host time is scaled
	virtual float		HostFrameTime() = 0; // host_frametime
	virtual float		HostTime() = 0; // host_time
	virtual int			HostTick() = 0; // host_tickcount
	virtual int			HostFrameCount() = 0; // total famecount

	virtual float		ServerTime() = 0; // gpGlobals->curtime on server
	virtual float		ServerFrameTime() = 0; // gpGlobals->frametime on server
	virtual int			ServerTick() = 0; // gpGlobals->tickcount on server
	virtual float		ServerTickInterval() = 0; // tick interval on server

	virtual float		ClientTime() = 0; // gpGlobals->curtime on client
	virtual float		ClientFrameTime() = 0; // gpGlobals->frametime on client
	virtual int			ClientTick() = 0; // gpGlobals->tickcount on client

	virtual void		SetClientFrameTime(float frametime) = 0; // gpGlobals->frametime on client

	// Currently the engine doesn't like to do networking when it's paused, but if a tool changes entity state, it can be useful to force 
	//  a network update to get that state over to the client
	virtual void		ForceUpdateDuringPause() = 0;

	// Maybe through modelcache???
	virtual void* GetModel() = 0;
	// Get the .mdl file used by entity (if it's a cbaseanimating)
	virtual void* GetStudioModel() = 0;

	// SINGLE PLAYER/LISTEN SERVER ONLY (just matching the client .dll api for this)
	// Prints the formatted string to the notification area of the screen ( down the right hand edge
	//  numbered lines starting at position 0
	virtual void		Con_NPrintf() = 0;
	// SINGLE PLAYER/LISTEN SERVER ONLY(just matching the client .dll api for this)
	// Similar to Con_NPrintf, but allows specifying custom text color and duration information
	virtual void		Con_NXPrintf() = 0;

	// Get the current game directory (hl2, tf2, hl1, cstrike, etc.)
	virtual void        GetGameDir(char* szGetGameDir, int maxlength) = 0;

	// Do we need separate rects for the 3d "viewport" vs. the tools surface??? and can we control viewports from
	virtual void		GetScreenSize(int& width, int& height) = 0;

	// GetRootPanel(VPANEL)

	// Sets the location of the main view
	virtual void		SetMainView() = 0;

	// Gets the player view
	virtual bool		GetPlayerView() = 0;

	// From a location on the screen, figure out the vector into the world
	virtual void		CreatePickingRay() = 0;

	// precache methods
	virtual bool		PrecacheSound(const char* pName, bool bPreload = false) = 0;
	virtual bool		PrecacheModel(const char* pName, bool bPreload = false) = 0;

	virtual void		InstallQuitHandler() = 0;
	virtual void		TakeTGAScreenShot(char const* filename, int width, int height) = 0;
	// Even if game is paused, force networking to update to get new server state down to client
	virtual void		ForceSend() = 0;

	virtual bool		IsRecordingMovie() = 0;

	// NOTE: Params can contain file name, frame rate, output avi, output raw, and duration
	virtual void		StartMovieRecording() = 0;
	virtual void		EndMovieRecording() = 0;
	virtual void		CancelMovieRecording() = 0;
	virtual void* GetActiveVideoRecorder() = 0;

	virtual void		StartRecordingVoiceToFile(char const* filename, char const* pPathID = 0) = 0;
	virtual void		StopRecordingVoiceToFile() = 0;
	virtual bool		IsVoiceRecording() = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void		TraceRay() = 0; // client version
	virtual void		TraceRayServer() = 0;

	virtual bool		IsConsoleVisible() = 0;

	virtual int			GetPointContents(const Vector3& vecPosition) = 0;

	virtual int			GetActiveDLights() = 0;
	virtual int			GetLightingConditions() = 0;

	virtual void		GetWorldToScreenMatrixForView() = 0;

	// Collision support
	virtual void CreatePartitionHandle() = 0;
	virtual void DestroyPartitionHandle() = 0;
	virtual void InstallPartitionQueryCallback() = 0;
	virtual void RemovePartitionQueryCallback() = 0;
	virtual void ElementMoved() = 0;
};