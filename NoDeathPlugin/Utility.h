// Supported with union (c) 2020 Union team
// Union HEADER file
#ifndef NODEATH_UTILITY
#define NODEATH_UTILITY

#include <vector>
#include <numeric>
#include <array>


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


template<std::size_t Size>
class FixedStr
{
	std::array<char, Size +1> m_array{};
	
public:

	constexpr FixedStr(const char(&source)[Size+1])
	{
		std::copy(std::begin(source), std::end(source), begin());
	}

	template<std::size_t LeftSize, std::size_t RightSize>
	constexpr FixedStr(const FixedStr<LeftSize>& t_left,const FixedStr<RightSize>& t_right)
	{
		static_assert(LeftSize + RightSize == Size);

		std::copy(t_left.cbegin(), t_left.cend(), begin());
		std::copy(t_right.cbegin(), t_right.cend(), begin() + t_left.size());
	}

	constexpr size_t size() const
	{
		return Size;
	}

	constexpr auto begin()
	{
		return m_array.data();
	}
		
	constexpr auto end()
	{
		return m_array.data() + size();
	}	
	
	constexpr auto cbegin() const
	{
		return m_array.data();
	}
		
	constexpr auto cend() const
	{
		return m_array.data() + size();
	}

	constexpr std::string_view data() const
	{
		return std::string_view{m_array.data(), m_array.size()};
	}

	template<std::size_t LeftSize, std::size_t RightSize>
	friend constexpr auto operator+(const FixedStr<LeftSize>& t_left, const FixedStr<RightSize>& t_right);
};

template<std::size_t Size>
FixedStr(const char(&)[Size]) -> FixedStr<Size - 1>;

template<std::size_t LeftSize, std::size_t RightSize>
FixedStr(FixedStr<LeftSize> t_left, FixedStr<RightSize> t_right) -> FixedStr<LeftSize + RightSize>;

template<std::size_t LeftSize, std::size_t RightSize>
constexpr auto operator+(const FixedStr<LeftSize>& t_left, const FixedStr<RightSize>& t_right)
{
	return FixedStr(t_left, t_right);
};


template<typename T, typename... Args> requires std::is_convertible_v<T, const char*> && (std::is_convertible_v<Args, const char*> && ...)
constexpr auto SetWaitMessage(T&& t_firstLine, Args&&... t_lines)
{
	const auto begin = FixedStr{ t_firstLine } + FixedStr{ "{:.2f}s" };
	
	if constexpr (sizeof...(t_lines) == 0)
	{
		return begin;
	}
	else
	{
		const auto end = ((FixedStr{ "\n" } + FixedStr{ t_lines }) + ...);
		return begin + end;
	}
}


std::string_view GetDefaultLocalizedMessage(const UnionCore::TSystemLangID t_id)
{
	using namespace UnionCore;

	static constexpr auto languages = std::make_tuple
	(
		//Lang_Eng
		SetWaitMessage("Wait ", "or press enter to continue..."), 
		//Lang_Pol
		SetWaitMessage("Poczekaj ", "lub naciœnij enter, by kontynuowaæ...")
	);

	switch (t_id)
	{
	case Lang_Eng: 
		return std::get<0>(languages).data();
	case Lang_Pol:
		return std::get<1>(languages).data();
	default:
		return std::get<0>(languages).data();
	}
	return {};
}

enum class eAfterDeath
{
	NOTHING, QUIT, NEWGAME, DELETEONLY
};

#endif