#include "player_info.h"

GlobalVarsBase* PlayerInfoManager::GetGlobalVars()
{
	using original_fn = GlobalVarsBase * (__fastcall*)(PlayerInfoManager*);
	return (*(original_fn**)this)[1](this);
}
