// Supported with union (c) 2020 Union team
// Union HEADER file

#include <filesystem>
#include <charconv>
#include <ranges>

namespace GOTHIC_ENGINE
{
	class SaveDeleter
	{
	public:

		SaveDeleter() = delete;
		SaveDeleter(SaveDeleter&) = delete;
		SaveDeleter& operator=(SaveDeleter&) = delete;

		[[nodiscard]]
		static bool IsSavePath(const std::filesystem::path& t_path, const std::vector<int>& t_saveSlots)
		{
			auto upper = [](const char ch)
			{ 
				return static_cast<char>(std::toupper(static_cast<unsigned char>(ch))); 
			};

			const auto upperPathName = t_path.filename().string() | std::views::transform(upper) | std::ranges::to<std::string>();
					
			if (upperPathName == "CURRENT")
			{
				return true;
			}

			if (upperPathName == "QUICKSAVE")
			{
				return true;
			}

			static constexpr std::string_view saveNameBegin{ "SAVEGAME" };


			if (upperPathName.starts_with(saveNameBegin))
			{
				int nr;

				const auto begin = std::next(upperPathName.c_str(), static_cast<std::ptrdiff_t>(saveNameBegin.size()));
				const auto end = std::next(upperPathName.c_str(), static_cast<std::ptrdiff_t>(upperPathName.size()));

				auto error = std::from_chars(begin, end, nr);

				if (error.ec != std::errc())
				{
					return false;
				}

				return std::ranges::any_of(t_saveSlots,
					[nr](const auto& slot)
					{
						return slot == nr;
					});
			}
			
			return false;
		}

		static void TryDelete()
		{
			const std::vector<int> slots = SaveDeleter::GetSavedSlots();

			if (slots.empty())
			{
				return;
			}

			try
			{
				const auto paths = SaveDeleter::GetPaths();

				bool deleted{};

				for (const auto& path : paths)
				{
					if (IsSavePath(path, slots))
					{
						const auto num = std::filesystem::remove_all(path);
						if (num > 1)
						{
							PrintLineCmd("deleted ", path.c_str(), " files ", num);

							deleted = true;
						}

					}
				}

				if (deleted)
				{
					ReinitSaveManager(slots);
				}
			}
			catch (const std::filesystem::filesystem_error& ex)
			{
				LogWarning(ex.what());
			}
		}

		[[nodiscard]]
		static std::vector<std::filesystem::path> GetPaths()
		{

			std::vector<std::filesystem::path> vec;


			static const auto savesString = []()
			{
				if (zoptions)
				{
					if (auto file = zoptions->GetDir(DIR_SAVEGAMES))
					{
						return std::filesystem::path{ file->GetFullPath().ToChar() };
					}
				}
				return std::filesystem::path{ L"..\\Saves\\" };

			}();

				try
				{
					for (const auto& entry : std::filesystem::directory_iterator(savesString))
					{
						if (entry.is_directory())
						{
							vec.push_back(entry.path());
						}
					}
				}
				catch (const std::filesystem::filesystem_error& ex)
				{
					PrintLineCmd(ex.what());
				}

				return vec;
		}


	private:

		[[nodiscard]]
		static std::vector<int> GetSavedSlots()
		{
			const auto saveManager = gameMan->savegameManager;

			if (!saveManager)
			{
				return{};
			}


			std::vector<int> vec; 
			vec.reserve(20);

			for (const auto info : zCArrayView(saveManager->infoList))
			{
				vec.push_back(info->m_SlotNr);
			}

			return vec;
		}

		static void ReinitSaveManager(const std::vector<int>& slots)
		{
			auto const saveMan = ogame->savegameManager;

			if (!saveMan)
			{
				return;
			}

			for (auto& info : zCArrayView(saveMan->infoList))
			{
				const auto nr = info->m_SlotNr;

				if (!std::ranges::any_of(slots,
					[nr](const auto& slot)
					{
						return slot == nr;
					}))
				{
					continue;
				}

				if (info->refCtr > 1)
				{
					LogWarning("saveMan->infoList[i]->refCtr > 1");
				}
				else
				{
					info->Release();
					info = new oCSavegameInfo(nr);
				}
			}

			saveMan->Reinit();
		}
	};
}
