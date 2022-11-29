#include "Injector.h"

#include "Offsets.h"

using namespace RE;

bool Injector::FileExists(const char* fileName)
{
	using func_t = decltype(&Injector::FileExists);
	REL::Relocation<func_t> func{ Offset::FileExists };
	return func(fileName);
}

void Injector::Register()
{
	UI* ui = UI::GetSingleton();
	if (ui) {
		ui->GetEventSource<MenuOpenCloseEvent>()->AddEventSink(this);
	}
}

BSEventNotifyControl Injector::ProcessEvent(const MenuOpenCloseEvent* event, MenuOpenCloseEventSource*)
{
	auto* strings = InterfaceStrings::GetSingleton();
	auto* ui = UI::GetSingleton();

	if (event->menuName == strings->hudMenu) {
		if (event->opening) {
			auto temp = ui->GetMovieView(strings->hudMenu);
			Inject(temp.get());
		}
	}
	return BSEventNotifyControl::kContinue;
}

void Injector::Inject(GFxMovieView* view)
{
	GFxValue args[2];
	GFxValue object;

	args[0].SetString("HUDHooksContainer");
	view->Invoke("getNextHighestDepth", &args[1], nullptr, 0);
	view->Invoke("createEmptyMovieClip", &object, args, 2);

	//BSScaleformManager* scaleform = BSScaleformManager::GetSingleton();

	// Determine which file is loaded for the hud menu
	if (this->FileExists("Interface/HUDMenu.swf")) {
		args[0].SetString("exported/HUDHooks.swf");
	} else if (this->FileExists("Interface/exported/HUDMenu.gfx")) {
		args[0].SetString("HUDHooks.swf");
	}

	object.Invoke("loadMovie", nullptr, &args[0], 1);
}
