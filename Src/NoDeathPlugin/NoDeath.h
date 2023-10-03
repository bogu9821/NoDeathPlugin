// Supported with union (c) 2020 Union team
// Union HEADER file
#include <chrono>
#include <string>
#include <ranges>

#ifndef AFTERDEATH_ENUM
#define AFTERDEATH_ENUM
enum class eAfterDeath : int
{
	NOTHING,
	QUIT,
	NEWGAME,
	DELETEONLY,
	MAX
};
#endif

namespace GOTHIC_ENGINE
{
	class NoDeath;
	extern std::unique_ptr<NoDeath> noDeath;
	
	class NoDeath
	{
	public:

		NoDeath(NoDeath&) = delete;
		NoDeath& operator=(NoDeath&) = delete;

		NoDeath()
		{
			CheckOptions();
		}

		void CheckOptions()
		{
			assert(zoptions);
			
			const auto realWaitTime = zoptions->ReadReal("NODEATH", "WaitTime", 5.f);

			m_waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(realWaitTime));

			m_blockInput = zoptions->ReadBool("NODEATH", "BlockInput", 1);

			const auto stepMin = std::to_underlying(eAfterDeath::NOTHING);
			const auto stepMax = std::to_underlying(eAfterDeath::MAX);

			auto readedStep = zoptions->ReadInt("NODEATH", "AfterDeath", stepMin);

			m_stepType = static_cast<eAfterDeath>(std::clamp(readedStep, stepMin, stepMax));

			m_compatibilityMode = zoptions->ReadInt("NODEATH", "CompatibilityMode", 1) != 0;
		}

		void Start(std::unique_ptr<FadeInScreen> t_fadeScreen)
		{
			m_fadeScreen = std::move(t_fadeScreen);

			m_startPoint = Clock::now();
			m_started = true;

		}

		[[nodiscard]]
		bool Started() const noexcept
		{
			return m_started;
		}

		[[nodiscard]]
		bool ShouldExit() const noexcept
		{
			return m_done && m_stepType == eAfterDeath::QUIT;
		}

		[[nodiscard]]
		bool ShouldRestart() const noexcept
		{
			return m_done && m_compatibilityMode && m_stepType == eAfterDeath::NEWGAME;
		}


		void OnLoop()
		{
			if (m_stepType == eAfterDeath::NOTHING || m_done)
			{
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
				using std::chrono::duration_cast;
				using std::chrono::milliseconds;

				const auto msElapsed = std::min(duration_cast<milliseconds>(Clock::now() - m_startPoint),m_waitTime);
	
				SaveDeleter::TryDelete();
				
				if (m_fadeScreen)
				{
					m_fadeScreen->Fade(msElapsed, m_waitTime, g_fadeMessage);
				}

				const SingleInputHelper optionalInputBlocker{ m_blockInput };

				if (msElapsed == m_waitTime || zKeyToggled(KEY_RETURN))
				{
					EndEffect();
				}

			}

		}

		private:

			void RestartGame()
			{

				if (m_compatibilityMode)
				{
					zoptions->WriteString("internal", "menuAction", "NEW_GAME", 0);

					const auto replaceParam = zoptions->commandline.Search("-NOMENU", 1) == -1;

					if (replaceParam)
					{
						zoptions->commandline = "-NOMENU" + zoptions->commandline;
					}

					gameMan->Menu(0);

					if (replaceParam)
					{
						zoptions->commandline.Delete("-NOMENU", zTSTR_KIND::zSTR_ONLY);
					}

				}
				else
				{
					const auto& worldName = ogame->GetGameWorld()->worldFilename;
					gameMan->GameSessionReset();

					ogame->LoadGame(-2, worldName);

					if (zmusic)
					{
						zmusic->Stop();
					}
					if (zsound)
					{
						zsound->StopAllSounds();
					}

					gameMan->playTime = {};

					//.data:008C2858                                   ; __time32_t Time

					auto Time = reinterpret_cast<__time32_t*>(0x008C2858);

					_time32(Time);
				}
			}

			void ExitGame()
			{
				if (m_compatibilityMode)
				{
					gameMan->ExitGame();
				}
				else
				{
					std::exit(-1);
				}
			}

			__declspec(noinline) void EndEffect()
			{
				m_fadeScreen.reset();

				m_done = 1;

				switch (m_stepType)
				{
				case eAfterDeath::QUIT:
					ExitGame();
					return;

				case eAfterDeath::NEWGAME:
					RestartGame();
					return;

				case eAfterDeath::NOTHING:
				case eAfterDeath::DELETEONLY:
					return;

				case eAfterDeath::MAX:
					std::unreachable();
				}

				assert(false);
			}


			std::unique_ptr<FadeInScreen> m_fadeScreen{};
			std::chrono::milliseconds m_waitTime{ 5000 };

			using Clock = std::chrono::high_resolution_clock;
			Clock::time_point m_startPoint{};
			bool m_started{};
			bool m_done{};
			bool m_compatibilityMode{true};
			eAfterDeath m_stepType{ eAfterDeath::NOTHING };

			bool m_blockInput{};


	};
}