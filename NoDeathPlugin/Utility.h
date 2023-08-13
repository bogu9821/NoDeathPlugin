// Supported with union (c) 2020 Union team
// Union HEADER file
#ifndef NODEATH_UTILITY
#define NODEATH_UTILITY

#include <vector>


template<typename... Args>
inline void PrintCmd(Args&&... args)
{
	((cmd << std::forward<Args>(args)), ...);
}

template<typename... Args>
inline void PrintLineCmd(Args&&... args)
{
	PrintCmd(std::forward<Args>(args)..., endl);
}


template<typename... Args>
inline void LogWarning(Args&&... args)
{
	PrintLineCmd("NoDeath :: ", std::forward<Args>(args)...);
}


constexpr std::string_view SystemLangIDToString(const UnionCore::TSystemLangID t_id)
{
	using namespace UnionCore;
	switch (t_id)
	{
	case Lang_Rus: return "Russian";
	case Lang_Eng: return "English";
	case Lang_Deu: return "German";
	case Lang_Pol: return "Polish";
	default:
		return "Other";
	}
}

constexpr std::string_view GetDefaultLocalizedMessage(const UnionCore::TSystemLangID t_id)
{
	using namespace UnionCore;
	switch (t_id)
	{
	case Lang_Eng: return "Wait {:.2f}s\n or press enter to continue...";
	case Lang_Pol: return "Poczekaj {:.2f}s\n lub naciœnij enter, by kontynuowaæ...";
	default:
		return "Wait {:.2f}s\n or press enter to continue...";
	}
}

enum class eAfterDeath
{
	NOTHING, QUIT, NEWGAME, DELETEONLY
};

#endif