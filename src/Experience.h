#pragma once

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

#include "Utils/Singleton.h"

class ExperienceManager : public ISingleton<ExperienceManager>
{
public:

	friend class ISingleton<ExperienceManager>;

	enum MeterMode
	{
		kHidden = 0,
		kDynamic = 1,
		kForced = 2
	};

	class Source
	{
	public:

		enum MeterState
		{
			kInactive = 0,
			kActive = 1
		};

		Source(ExperienceManager* manager, MeterState meter) :
			manager(manager), meter(meter) {};

		virtual ~Source() {};

		void AddExperience(int points);

	private:
		ExperienceManager* manager;
		MeterState         meter;
	};

	struct Experience
	{
		Source* source{ nullptr };
		int     points;
		int     widget;
	};

	void AddExperience(const Experience& xp);
	void AddExperience(int points, bool meter = false);
	float GetExperience();

	void Init();
	void Stop() { running = false; };

private:
	using clock_t = std::chrono::steady_clock;
	using timepoint_t = clock_t::time_point;
	using ExperienceSources = std::vector<std::unique_ptr<Source>>;
	using PlayerSkills = RE::PlayerCharacter::PlayerSkills;
	using LevelData = PlayerSkills::Data;

	ExperienceManager();
	~ExperienceManager() override;

	void Loop();
	void Process(std::unique_lock<std::mutex>& lock);

	bool ShouldDisplayLevelMeter(bool display);
	void ShowLevelMeter(float start, float end);
	void ShowLevelUpNotification();
	void ShowRewardMessage(int points);

	// members
	ExperienceSources        events;
	std::queue<Experience>   queue;
	mutable std::mutex       mtx;
	std::condition_variable  wakeup;
	std::thread              worker;
	std::atomic<bool>        running{ true };
	timepoint_t              timeout;
	LevelData*               data;
};
