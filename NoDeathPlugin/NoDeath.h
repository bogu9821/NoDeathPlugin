// Supported with union (c) 2020 Union team
// Union HEADER file

#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <optional>
#include <ranges>

enum class eAfterDeath;

namespace GOTHIC_ENGINE
{
	struct SingleInputHelper
	{
		SingleInputHelper() { zinput->ProcessInputEvents(); }
		~SingleInputHelper() { zinput->ClearKeyBuffer(); }
	};


	class NoDeath
	{
	public:

		NoDeath()
		{

			const auto realWaitTime = zoptions->ReadReal("NODEATH", "WaitTime", 5.f);

			m_waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(realWaitTime));

			m_blockInput = zoptions->ReadBool("NODEATH", "BlockInput", 1);

			auto readedStep = zoptions->ReadString("NODEATH", "AfterDeath", "NOTHING");

			const std::string_view view{ readedStep.ToChar() };


			if (view == "NEWGAME")
			{
				m_stepType = eAfterDeath::NEWGAME;
			}
			else if (view == "QUIT")
			{
				m_stepType = eAfterDeath::QUIT;
			}
			else if (view == "DELETEONLY")
			{
				m_stepType = eAfterDeath::DELETEONLY;
			}
			else
			{
				m_stepType = eAfterDeath::NOTHING;

				if (view != "NOTHING")
				{
					LogWarning("probably bad end step ", view.data());
					zoptions->WriteString("NODEATH", "AfterDeath", "NOTHING", 0);
				}
			}

		}

		void Start(std::unique_ptr<FadeInScreen> t_fadeScreen)
		{
			m_fadeScreen = std::move(t_fadeScreen);

			m_startPoint = Clock::now();
			m_started = true;

		}

		bool Started() const noexcept
		{
			return m_started;
		}

		bool ShouldExit() const noexcept
		{
			return m_done && m_stepType == eAfterDeath::QUIT;
		}

		void OnLoop()
		{
			if (m_stepType == eAfterDeath::NOTHING)
				return;
			
			if (m_done)
			{
				//game not exited?
				if (ShouldExit())
				{
					std::exit(-1);
				}

				return;
			}

			if (Started() == false)
			{
				if (player && player->GetAttribute(NPC_ATR_HITPOINTS) <= 0)
				{
					Start(std::make_unique<FadeInScreen>("black"));
				}

			}
			else
			{

				const auto msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_startPoint);

				
				SaveDeleter::TryDelete();

				const auto optionalInputBlocker = m_blockInput ? std::make_unique<SingleInputHelper>() : nullptr;


				

				if (msElapsed >= m_waitTime || zKeyToggled(KEY_RETURN))
				{
					EndEffect();
					return;
				}


				if (m_fadeScreen)
				{
					const auto language = UnionCore::Union.GetSystemLanguage();

					const auto it = std::ranges::find_if(LocalizedMessage::s_endMessages,
						[language](const auto& message) -> bool
						{
							return message.m_id == language;
						});




					const auto& text = it == std::end(LocalizedMessage::s_endMessages) ? "" : it->m_message;

					m_fadeScreen->Fade(msElapsed, m_waitTime, text);

				}

			}

		}

		private:
			void EndEffect()
			{
				m_fadeScreen.reset();

				m_done = 1;

				if (m_stepType == eAfterDeath::QUIT)
				{
					gameMan->ExitGame();
				}
				else if (m_stepType == eAfterDeath::NEWGAME)
				{
					//zoptions->WriteString("internal", "menuAction", "NEW_GAME",0);
					//zoptions->AddParameters("-NOMENU");
					//gameMan->Menu(0);

					//gameMan->ExitSession();

					const auto& worldName = ogame->GetGameWorld()->worldFilename;
					gameMan->GameSessionReset();

					ogame->LoadGame(-2, worldName);

					if (zmusic)
						zmusic->Stop();
					if (zsound)
						zsound->StopAllSounds();

					gameMan->playTime = {};

					//.data:008C2858                                   ; __time32_t Time

					auto Time = reinterpret_cast<__time32_t*>(0x008C2858);

					_time32(Time);
				}
			}


			std::unique_ptr<FadeInScreen> m_fadeScreen{};
			std::chrono::milliseconds m_waitTime{ 5000 };

			using Clock = std::chrono::high_resolution_clock;
			Clock::time_point m_startPoint{};
			bool m_started{};
			bool m_done{};
			eAfterDeath m_stepType{ eAfterDeath::NOTHING };

			bool m_blockInput{};


	};
}