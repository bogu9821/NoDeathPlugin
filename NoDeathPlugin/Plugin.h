
// This file added in headers queue
// File: "Headers.h"

namespace GOTHIC_ENGINE {
	
	std::unique_ptr<NoDeath> noDeath{};

    void ResetNoDeath()
    {
        if (!SaveLoadGameInfo.changeLevel)
        {
            if (noDeath->ShouldReset())
            {
                noDeath = std::make_unique<NoDeath>();
            }
        }
    }
}
