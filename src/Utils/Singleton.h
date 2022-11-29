#pragma once

template <typename T>
class Singleton
{
public:
	static T& GetSingleton()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() noexcept = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	virtual ~Singleton() = default;
};
