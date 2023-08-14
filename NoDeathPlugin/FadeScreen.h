// Supported with union (c) 2020 Union team
// Union HEADER file

#include <string>
#include <algorithm>
#include <chrono>
#include <ranges>
#include <format>



namespace GOTHIC_ENGINE
{
	class zSTRING_back_insert_iterator
	{
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = zSTRING;

#ifdef __cpp_lib_concepts
		using difference_type = ptrdiff_t;
#else
		using difference_type = void;
#endif // __cpp_lib_concepts

		_CONSTEXPR20 explicit zSTRING_back_insert_iterator(zSTRING& _Cont) noexcept /* strengthened */
			: container(_STD addressof(_Cont)) {}

		zSTRING_back_insert_iterator& operator=(char _Val) {
			container->Align(zSTR_LEFT, container->Length()+1, _Val);
			return *this;
		}


		_NODISCARD _CONSTEXPR20 zSTRING_back_insert_iterator& operator*() noexcept /* strengthened */ {
			return *this;
		}

		_CONSTEXPR20 zSTRING_back_insert_iterator& operator++() noexcept /* strengthened */ {
			return *this;
		}

		_CONSTEXPR20 zSTRING_back_insert_iterator operator++(int) noexcept /* strengthened */ {
			return *this;
		}

	protected:
		zSTRING* container;
	};


	template<typename ...Args>
	inline zSTRING FormatString(const std::string_view t_text, Args&&... t_args)
	{
		std::basic_format_args<std::format_context> args = std::make_format_args(std::forward<Args>(t_args)...);
		
		const auto size = args._Estimate_required_capacity() + 1;
		auto buffer = std::make_unique<char[]>(size);
		buffer[size - 1] = '\0';

		//no reserve method
		zSTRING formattedText{ buffer.get() };
		
		std::vformat_to(zSTRING_back_insert_iterator{ formattedText }, t_text, args);

		return formattedText;
	}

	class FadeInScreen
	{
	public:

		FadeInScreen(FadeInScreen&) = delete;
		FadeInScreen& operator=(FadeInScreen&) = delete;

		FadeInScreen(const std::string_view t_texture) : m_view(std::make_unique<zCView>())
		{
			m_view->SetSize(8192, 8192);
			m_view->SetPos(0, 0);
			m_view->InsertBack(t_texture.data());
			SetAlpha(0);
			m_view->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
			m_view->SetFont("FONT_OLD_10_WHITE_HI.TGA");
		}	

		void Fade(const std::chrono::milliseconds t_timeElapsed, const std::chrono::milliseconds t_timeEnd, const std::string_view t_message)
		{
			const auto fadeTick = t_timeEnd.count() / 255;
			const auto ticksElapsed = fadeTick != 0 ? t_timeElapsed.count() / fadeTick : 0;

			SetAlpha(static_cast<int>(std::clamp(ticksElapsed, 0ll, 255ll)));


			screen->InsertItem(m_view.get());

			const auto floatDuration = std::chrono::duration_cast<std::chrono::duration<float>>(t_timeEnd - t_timeElapsed);

			const auto toEnd = floatDuration.count() > 0.f ? floatDuration.count() : 0.f;


			Print(toEnd, t_message);
			m_view->Render();

			screen->RemoveItem(m_view.get());


		}

	private:
		void Print(const float t_seconds, const std::string_view t_text)
		{
			const auto formattedText = FormatString(t_text,t_seconds);
			m_view->ClrPrintwin();

			for (uint32_t i = 1; ; i++)
			{
				auto line = formattedText.PickWord(i, "\n", "\n");
				if (line.IsEmpty())
				{
					break;
				}

				const auto fontSize = m_view->FontSize(line);
				const auto x = 4096 - fontSize / 2;
				const int y = 4096 + (m_view->FontY() * m_view->textLines.GetNum());

				zCOLOR color(255, 255, 255, m_view->alpha);

				m_view->CreateText(x, y, line, 0.f, color, 0, 1);

			}

		}

		inline void SetAlpha(const int t_alpha)
		{
			m_view->SetTransparency(t_alpha);
		}


		std::unique_ptr<zCView> m_view{};

	};
}