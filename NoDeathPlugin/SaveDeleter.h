// Supported with union (c) 2020 Union team
// Union HEADER file

#include <vector>
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

		static bool IsSavePath(const std::filesystem::path& t_path, const std::vector<int>& t_saveSlots)
		{
			auto upper = [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); };
	
			const auto pathName = t_path.filename().string();
			const auto upperPathName = pathName | (std::views::transform(upper) | std::ranges::to<std::string>());

			
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

				const auto begin = upperPathName.c_str() + saveNameBegin.size();
				const auto end = upperPathName.c_str() + upperPathName.size();

				auto error = std::from_chars(begin, end, nr);

				if (error.ec != std::errc())
				{
					return false;
				}

				return std::ranges::any_of(t_saveSlots, [nr](const auto& slot) { return slot == nr; });
			}

			return false;
		}

		static bool TryDelete()
		{
			const std::vector<int> slots = SaveDeleter::GetSavedSlots();

			if (slots.empty())
			{
				return false;
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
						if (num > 0)
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
			catch (std::exception& ex)
			{
				LogWarning(ex.what());
			}

			return 1;
		}


		static std::vector<std::filesystem::path> GetPaths()
		{

			std::vector<std::filesystem::path> vec;


			static const auto savesString = []() -> std::filesystem::path
			{

				if (zoptions)
				{	
					auto file = zoptions->GetDir(DIR_SAVEGAMES);
					if (file)
						return file->GetFullPath().ToChar();
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
			catch (std::exception& ex)
			{
				PrintLineCmd(ex.what());
			}

			return vec;
		}


		private:
			static std::vector<int> GetSavedSlots()
			{

				const auto saveManager = gameMan->savegameManager;

				if (!saveManager)
					return{};


				std::vector<int> vec; vec.reserve(20);

				const auto& infoList = saveManager->infoList;

				for (const auto i : std::views::iota(0, infoList.GetNum()))
				{
					vec.push_back(infoList[i]->m_SlotNr);
				}

				return vec;
			}

			static void ReinitSaveManager(const std::vector<int>& slots)
			{
				if (ogame->savegameManager)
				{
					auto const saveMan = ogame->savegameManager;

					for (int i = 0; i < saveMan->infoList.GetNum(); i++)
					{
						const auto nr = saveMan->infoList[i]->m_SlotNr;

						if (!std::ranges::any_of(slots, [nr](const auto& slot) { return slot == nr; }))
							continue;

						if (saveMan->infoList[i]->refCtr > 1)
						{
							LogWarning("saveMan->infoList[i]->refCtr > 1");
						}
						else
						{
							saveMan->infoList[i]->Release();
							saveMan->infoList[i] = new oCSavegameInfo(nr);
						}
					}

					saveMan->Reinit();

				}
			}
	};
}