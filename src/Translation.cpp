#include "Translation.h"

using namespace RE;

std::string Translation::WideStringToString(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.length(), &strTo[0], size_needed, nullptr, nullptr);
	return strTo;
}

std::wstring Translation::StringToWideString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.length(), nullptr, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.length(), &wstrTo[0], size_needed);
	return wstrTo;
}

auto Translation::GetScaleformTranslator(GFxLoader* loader) 
	-> GPtr<BSScaleformTranslator>
{
	return loader->GetState<BSScaleformTranslator>(StateType::kTranslator);
}

std::string Translation::Translate(const std::string& key)
{
	if (key.empty() || !key.starts_with('$')) {
		return key;
	}

	auto manager = BSScaleformManager::GetSingleton();
	auto loader = manager->loader;
	auto translator = GetScaleformTranslator(loader);

	std::wstring wkey = StringToWideString(key);

	TranslateInfo info;
	GFxWStringBuffer out;

	info.key = wkey.c_str();
	info.result = &out;

	translator->Translate(&info);

	std::string result = WideStringToString(out.c_str());

	logger::trace("Translated: {0} -> {1}", key, result);

	return result;
}

