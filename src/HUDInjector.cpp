#include "HUDInjector.h"

#include "Offsets.h"

using namespace RE;


void HUDInjector::Register()
{
	if (auto ui = UI::GetSingleton()) {
		ui->GetEventSource<MenuOpenCloseEvent>()->AddEventSink(this);
	}
}

BSEventNotifyControl HUDInjector::ProcessEvent(const MenuOpenCloseEvent* event, MenuOpenCloseEventSource*)
{
	auto strings = InterfaceStrings::GetSingleton();
	auto ui = UI::GetSingleton();

	if (event->menuName == strings->hudMenu && event->opening) {
		auto ptr = ui->GetMovieView(strings->hudMenu);
		Inject(ptr.get());
	}
	return BSEventNotifyControl::kContinue;
}

void HUDInjector::Inject(GFxMovieView* view)
{
	GFxValue args[2], object, path;

	args[0].SetString("HUDHooksContainer");
	view->Invoke("getNextHighestDepth", &args[1], nullptr, 0);
	view->Invoke("createEmptyMovieClip", &object, args, 2);

	// Determine which file is loaded for the hud menu
	if (BSScaleformManager::FileExists("Interface/HUDMenu.swf")) {
		path.SetString("exported/HUDHooks.swf");
	} else if (BSScaleformManager::FileExists("Interface/exported/HUDMenu.gfx")) {
		path.SetString("HUDHooks.swf");
	}

	object.Invoke("loadMovie", nullptr, &path, 1);
}
