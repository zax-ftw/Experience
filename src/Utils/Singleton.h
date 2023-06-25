#pragma once

template <class T>
class ISingleton
{
public:
	static T* GetSingleton()
	{
		static T instance;
		return std::addressof(instance);
	}

protected:
	ISingleton() noexcept = default;
	ISingleton(const ISingleton&) = delete;
	ISingleton(ISingleton&&) = delete;
	ISingleton& operator=(const ISingleton&) = delete;
	ISingleton& operator=(ISingleton&&) = delete;
	virtual ~ISingleton() = default;
};
