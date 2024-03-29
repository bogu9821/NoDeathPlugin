﻿// Supported with union (c) 2020 Union team
// Union HEADER file
#include <vector>
#include <numeric>
#include <array>
#include <format>
#include <ranges>
#include <algorithm>

namespace GOTHIC_ENGINE
{
	template<typename ...Args>
	[[nodiscard]]
	inline zSTRING FormatString(const std::string_view t_text, Args&&... t_args)
	{
		const auto formattedStr = std::vformat(t_text, std::make_format_args(std::forward<Args>(t_args)...));
		zSTRING formattedText{ formattedStr.c_str() };

		return formattedText;
	}

	struct SingleInputHelper
	{
		bool m_blocked{};

		explicit SingleInputHelper(bool t_block)
			: m_blocked(t_block)
		{
			if (t_block)
			{
				zinput->ProcessInputEvents();
			}
		}

		~SingleInputHelper()
		{
			if (m_blocked)
			{
				zinput->ClearKeyBuffer();
			}
		}
	};


	template<typename T>
	inline auto zCArrayView(zCArray<T>&  t_array)
	{
		return std::span{ t_array.GetArray(),static_cast<size_t>(t_array.GetNum()) };
	}
}

#ifndef NODEATH_UTILITY
#define NODEATH_UTILITY

template<typename... Args>
inline void PrintCmd(Args&&... args)
{
	(void)(((cmd << std::forward<Args>(args))), ...);
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

[[nodiscard]]
inline constexpr char CharToUpperSimple(const char t_char)
{
	return t_char >= 'a' && t_char <= 'z'
		? static_cast<char>(static_cast<unsigned char>(t_char) - 'a' + 'A')
		: t_char;
}

[[nodiscard]]
inline constexpr char CharToLowerSimple(const char t_char)
{
	return t_char >= 'A' && t_char <= 'Z'
		? static_cast<char>(static_cast<unsigned char>(t_char) + ('a' - 'A'))
		: t_char;
}

template<std::size_t Size>
class FixedStr
{
	std::array<char, Size + 1> m_array{};

public:

	constexpr FixedStr(const char(&source)[Size + 1])
	{
		std::copy(std::cbegin(source), std::cend(source), begin());
	}

	template<std::size_t LeftSize, std::size_t RightSize>
	constexpr FixedStr(const FixedStr<LeftSize>& t_left, const FixedStr<RightSize>& t_right)
	{
		static_assert(LeftSize + RightSize == Size);

		std::copy(t_left.cbegin(), t_left.cend(), begin());
		std::copy(t_right.cbegin(), t_right.cend(), std::next(begin(), static_cast<std::ptrdiff_t>(t_left.size())));
	}

	constexpr auto& upper()
	{
		std::ranges::transform(m_array, std::begin(m_array), CharToUpperSimple);
		return *this;
	}

	constexpr auto& lower()
	{
		std::ranges::transform(m_array, std::begin(m_array), CharToLowerSimple);
		return *this;
	}

	[[nodiscard]]
	constexpr size_t size() const noexcept
	{
		return Size;
	}

	[[nodiscard]]
	constexpr char* begin()
	{
		return m_array.data();
	}

	[[nodiscard]]
	constexpr char* end()
	{
		return std::next(m_array.data(), size());
	}

	[[nodiscard]]
	constexpr const char* cbegin() const
	{
		return m_array.data();
	}

	[[nodiscard]]
	constexpr const char* cend() const
	{
		return std::next(m_array.data(), static_cast<std::ptrdiff_t>(size()));
	}

	[[nodiscard]]
	constexpr std::string_view data() const noexcept
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
[[nodiscard]]
constexpr auto SetWaitMessage(T&& t_firstLine, Args&&... t_lines)
{
	const auto begin = FixedStr{ std::forward<T>(t_firstLine) } + FixedStr{ "{:.2f}s" };

	if constexpr (sizeof...(t_lines) == 0)
	{
		return begin;
	}
	else
	{
		const auto end = ((FixedStr{ "\n" } + FixedStr{ std::forward<Args>(t_lines) }) + ...);
		return begin + end;
	}
}

#endif
