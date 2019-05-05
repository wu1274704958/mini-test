#include <iostream>
#include <dbg.hpp>
#include "ta.h"

#include <vector>
#include <thread>
#include <atomic>

int main()
{
	Ta ta;
	std::atomic<int> t(0);
	dbg((int)t);
	dbg(t.is_lock_free());
	dbg(t.is_always_lock_free);
	std::vector<std::thread> ts;

	auto f = [&t]() mutable {
		for (int i = 0; i < 1000000; ++i)
		{
			t++;
		}
	};

	for (int i = 0; i < 10; i++)
	{
		ts.push_back(std::thread(f));
	}

	for (auto& t : ts)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	

	dbg((int)t);

#ifdef WIN32
	system("pause");
#endif
	return 0;
}