#pragma once

#include "Utils/Singleton.h"


class HUDInjector : public ISingleton<HUDInjector>,
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	using MenuOpenCloseEventSource = RE::BSTEventSource<RE::MenuOpenCloseEvent>;

	friend class ISingleton<HUDInjector>;

	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, MenuOpenCloseEventSource* source);	

	void Register();

private:
	HUDInjector() = default;

	void Inject(RE::GFxMovieView* view);
};
