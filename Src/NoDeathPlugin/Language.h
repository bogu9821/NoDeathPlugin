﻿// Supported with union (c) 2020 Union team
// Union HEADER file
#ifndef NODEATH_LANGUAGES_INCLUDE
#define NODEATH_LANGUAGES_INCLUDE

inline constexpr auto g_languages = std::make_tuple
(
	std::make_pair(Lang_Eng, SetWaitMessage("Wait ", "or press enter to continue...")),
	std::make_pair(Lang_Pol, SetWaitMessage("Poczekaj ", "lub naciśnij enter, by kontynuować...")),
	std::make_pair(Lang_Deu, SetWaitMessage("Warte", "oder drücke Enter, um fortzufahren...")),
	std::make_pair(Lang_Rus, SetWaitMessage("\xCF\xEE\xE4\xEE\xE6\xE4\xE8\xF2\xE5 ", "\xE8\xEB\xE8 \xED\xE0\xE6\xEC\xE8\xF2\xE5 Enter, \xF7\xF2\xEE\xE1\xFB \xEF\xF0\xEE\xE4\xEE\xEB\xE6\xE8\xF2\xFC..."))
);

inline std::string_view g_fadeMessage{};

[[nodiscard]]
constexpr std::string_view SystemLangIDToString(const UnionCore::TSystemLangID t_id)
{
	using namespace UnionCore;
	switch (t_id)
	{
	case Lang_Rus:
		return "Russian";
	case Lang_Eng:
		return "English";
	case Lang_Deu:
		return "German";
	case Lang_Pol:
		return "Polish";
	default:
		return "Other";
	}
}

[[nodiscard]]
__declspec(noinline) std::string_view GetDefaultLocalizedMessage(const UnionCore::TSystemLangID t_id)
{
	return std::apply
	(
		[t_id](const auto& ... args)  -> std::string_view
		{
			std::string_view s;

			(((t_id == args.first) ? (s = args.second.data(), true) : false)
				|| ... || (s = std::get<0>(g_languages).second.data(), true));

			return s;


		}, g_languages
	);

}

#endif
