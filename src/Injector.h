#pragma once

class Injector : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	using MenuOpenCloseEventSource = RE::BSTEventSource<RE::MenuOpenCloseEvent>;

	Injector(const Injector&) = delete;
	Injector& operator=(const Injector&) = delete;

	static Injector& GetSingleton()
	{
		static Injector singleton;
		return singleton;
	};

	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, MenuOpenCloseEventSource* source);	

	void Register();

private:
	Injector() = default;

	void Inject(RE::GFxMovieView* view);
	static bool FileExists(const char* fileName);
};
