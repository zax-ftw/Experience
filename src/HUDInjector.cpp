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

		if (auto task = SKSE::GetTaskInterface()) {

			task->AddUITask([=] {
				auto view = ui->GetMovieView(strings->hudMenu);
				Inject(view);	
			});
		}
	}
	return BSEventNotifyControl::kContinue;
}

void HUDInjector::Inject(GPtr<GFxMovieView> view)
{
	if (view && view->GetVisible()) {

		GFxValue args[2], object, path;

		args[0].SetString("HUDHooksContainer");
		view->Invoke("getNextHighestDepth", &args[1], nullptr, 0);
		view->Invoke("createEmptyMovieClip", &object, args, 2);

		if (BSScaleformManager::FileExists("Interface/HUDMenu.swf")) {
			path.SetString("exported/HUDHooks.swf");
		} else if (BSScaleformManager::FileExists("Interface/exported/HUDMenu.gfx")) {
			path.SetString("HUDHooks.swf");
		}

		object.Invoke("loadMovie", nullptr, &path, 1);
	}
}
