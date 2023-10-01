// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	HOOK Hook_CGameManager__IsGameRunning PATCH(&CGameManager::IsGameRunning,&CGameManager::IsGameRunning_NoDeath);
	int CGameManager::IsGameRunning_NoDeath()
	{
		if (noDeath->ShouldReset())
		{
			return 0;
		}
		
		return THISCALL(Hook_CGameManager__IsGameRunning)();
	}
}
