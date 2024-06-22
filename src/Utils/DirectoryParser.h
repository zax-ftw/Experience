
namespace Utils
{
	namespace fs = std::filesystem;

	void ParseFile(const fs::path& file, std::unordered_map<RE::TESForm*, int>& data)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		SI_Error rc = ini.LoadFile(file.c_str());
		if (rc < 0) {
			logger::warn("Failed to parse file: {}", file.filename().string());
			return;
		}

		logger::info("Parsing file: {}", file.filename().string());

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
		for (const auto& section : sections) {
			CSimpleIniA::TNamesDepend keys;
			ini.GetAllKeys(section.pItem, keys);

			for (const auto& key : keys) {
				RE::FormID formID = std::strtol(key.pItem, nullptr, 16);

				auto form = dataHandler->LookupForm(formID, section.pItem);
				if (form) {
					int value = ini.GetLongValue(section.pItem, key.pItem);
					logger::trace("{0} | {1:08X} | {2}", section.pItem, formID, value);

					data[form] = value;
				}
			}
		}
	}

	void ParseDirectory(const fs::path& root, std::unordered_map<RE::TESForm*, int>& data)
	{
		using std::chrono::duration_cast;
		using std::chrono::milliseconds;
		using std::chrono::steady_clock;

		auto t1 = steady_clock::now();

		if (fs::exists(root) && fs::is_directory(root)) {
			for (auto& entry : fs::directory_iterator(root)) {
				if (entry.is_regular_file() && entry.path().extension() == ".ini") {
					ParseFile(entry.path(), data);
				}
			}
		}

		auto t2 = steady_clock::now();
		auto ms = duration_cast<milliseconds>(t2 - t1);

		logger::info("Directory '{}' parsed in {} ms", root.string(), ms.count());
	}

}
