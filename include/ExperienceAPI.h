#pragma once


namespace Experience
{
	enum ApiVersion
	{
		V1,

		Current = V1
	};

	class InterfaceVersion1
	{
	public:

		inline static constexpr auto Version = ApiVersion::V1;

		virtual ~InterfaceVersion1() = default;

		/// <summary>
		/// Gets the current version of the interface.
		/// </summary>
		/// <returns></returns>
		virtual ApiVersion GetVersion() = 0;

		/// <summary>
		/// Adds player experience, optionally displaying progress meter.
		/// </summary>
		/// <param name="points">Experience points to add.</param>
		/// <param name="meter">Display meter or not.</param>
		virtual void AddExperience(float points, bool meter) = 0;

		/// <summary>
		/// Gets the current player experience points.
		/// </summary>
		/// <returns></returns>
		virtual float GetExperience() = 0;

		/// <summary>
		/// Gets the cap for specified actor value.
		/// </summary>
		/// <returns></returns>
		virtual float GetSkillCap(RE::ActorValue skill) = 0;

		/// <summary>
		/// Displays middle of screen notification.
		/// </summary>
		/// <param name="text">Notification text.</param>
		/// <param name="status">Notification status.</param>
		/// <param name="sound">EditorID of sound descriptor.</param>
		/// <returns></returns>
		virtual void ShowNotification(const char* text, const char* status, const char* sound) = 0;
	};

	namespace detail
	{
		typedef void*(WINAPI* _RequestAPI)(ApiVersion);

		inline void* RequestAPI(ApiVersion version) noexcept
		{
			HMODULE handle = GetModuleHandle("Experience.dll");
			if (handle) {
				_RequestAPI func = (_RequestAPI)GetProcAddress(handle, "RequestAPI");
				if (func) {
					return func(version);
				}
			}
			return nullptr;
		}
	}

	/// <summary>
	/// Accesses the Experience API Interface. Safe to call PostPostLoad.
	/// </summary>
	/// <returns>Casts to and returns a specific version of the interface.</returns>
	template <class Interface = InterfaceVersion1>
	inline Interface* RequestAPI()
	{
		return reinterpret_cast<Interface*>(
			detail::RequestAPI(Interface::Version));
	}

}
