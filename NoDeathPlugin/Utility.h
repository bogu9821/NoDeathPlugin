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
	case Lang_Eng: return "Wait {}s\n or press enter to continue...";
	case Lang_Pol: return "Poczekaj {}s\n lub naciœnij enter, by kontynuowaæ...";
	default:
		return "";
	}
}


struct LocalizedMessage
{
	LocalizedMessage(std::string t_message, UnionCore::TSystemLangID t_id) : m_message(std::move(t_message)), m_id(t_id) {}

	static void Init()
	{
		s_endMessages.clear();

		for (int i = 0; i < UnionCore::TSystemLangID::Lang_Max; i++)
		{
			const auto id = static_cast<UnionCore::TSystemLangID>(i);

			const std::string msg{ GetDefaultLocalizedMessage(id) };

			if (msg.empty() == false)
			{
				s_endMessages.emplace_back(msg, id);
			}
		}
	}

	inline static std::vector<LocalizedMessage> s_endMessages{};

	std::string m_message{};
	UnionCore::TSystemLangID m_id{};
};

enum class eAfterDeath
{
	NOTHING, QUIT, NEWGAME, DELETEONLY
};



#endif