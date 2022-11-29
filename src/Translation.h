#pragma once

namespace Translation
{
	using TranslateInfo = RE::GFxTranslator::TranslateInfo;
	using StateType = RE::GFxState::StateType;

	[[nodiscard]] std::string WideStringToString(const std::wstring& wstr);
	[[nodiscard]] std::wstring StringToWideString(const std::string& str);
	[[nodiscard]] std::string Translate(const std::string& key);

	auto GetScaleformTranslator(RE::GFxLoader* loader) -> RE::GPtr<RE::BSScaleformTranslator>;
}
