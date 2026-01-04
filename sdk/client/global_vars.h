#pragma once

#define TICK_INTERVAL		( Interfaces.Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )	( static_cast<int>( (TICK_INTERVAL / 2) + static_cast<float>(dt) / TICK_INTERVAL ) )
#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME( t )	( TICK_INTERVAL * ( t ) )
#endif

class GlobalVarsBase {
public:
	float     realtime;
	int       framecount;
	float     absoluteframetime;
	float     curtime;
	float     frametime;
	int       max_clients;
	int       tickcount;
	float     interval_per_tick;
	float     interpolation_amount;
	int       sim_ticks_this_frame;
	int       network_protocol;
	void*	  save_data;
	bool      m_bclient;
	bool      m_bremoteclient;
};