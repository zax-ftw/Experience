#pragma once

#include <mutex>
#include <queue>

template <class T>
class SafeQueue
{
public:
	SafeQueue(void) :
		q(), m()
	{}

	void push(const T& elem)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(elem);
	}

	bool pop(T& elem) noexcept
	{
		std::lock_guard<std::mutex> lock(m);
		if (q.empty()) {
			return false;
		}
		elem = q.front();
		q.pop();
		return true;
	}

private:
	std::queue<T>      q;
	mutable std::mutex m;
};
