// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	HOOK Hook_CGameManager__IsGameRunning AS(&CGameManager::IsGameRunning,&CGameManager::IsGameRunning_NoDeath);
	int CGameManager::IsGameRunning_NoDeath()
	{
		const auto ret = THISCALL(Hook_CGameManager__IsGameRunning)();
		
		if (ret && noDeath && noDeath->ShouldRestart())
		{
			return 0;
		}
		
		return ret;
	}
}
