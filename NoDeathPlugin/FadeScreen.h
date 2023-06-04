// Supported with union (c) 2020 Union team
// Union HEADER file

#include <string>
#include <algorithm>
#include <chrono>
#include <ranges>
#include <format>

namespace GOTHIC_ENGINE
{
	class FadeInScreen
	{
	public:

		FadeInScreen(const std::string_view t_texture) : m_view(new zCView)
		{
			m_view->SetSize(8192, 8192);
			m_view->SetPos(0, 0);
			m_view->InsertBack(t_texture.data());
			SetAlpha(0);
			m_view->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
			m_view->SetFont("FONT_OLD_10_WHITE_HI.TGA");
		}
		~FadeInScreen()
		{
			delete m_view;
		}

		

		void Fade(const std::chrono::milliseconds t_timeElapsed, const std::chrono::milliseconds t_timeEnd, const std::string& message)
		{
			const auto fadeTick = t_timeEnd.count() / 255;
			const auto ticksElapsed = fadeTick != 0 ? t_timeElapsed.count() / fadeTick : 0;

			SetAlpha(static_cast<int>(std::clamp(ticksElapsed, 0ll, 255ll)));


			screen->InsertItem(m_view);

			const auto floatDuration = std::chrono::duration_cast<std::chrono::duration<float>>(t_timeEnd - t_timeElapsed);

			const auto toEnd = floatDuration.count() > 0.f ? floatDuration.count() : 0.f;


			Print(toEnd, message);
			m_view->Render();

			screen->RemoveItem(m_view);


		}

	private:
		void Print(const float t_seconds, const std::string& t_text)
		{
			static constexpr std::string_view separator{ "{}" };

			const auto secondsIndex = t_text.find(separator);

			if (secondsIndex == std::string::npos)
				return;

			const auto nextWord = secondsIndex + separator.size();

			const std::string text_begin{ t_text.c_str(), secondsIndex };
			const zSTRING text_replaced{ t_seconds, 2 };
			const std::string_view text_end{ t_text.c_str() + nextWord };

			zSTRING formattedText = text_begin.c_str() + text_replaced + text_end.data();

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


		zCView* m_view{};

	};
}
