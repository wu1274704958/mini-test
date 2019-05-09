#include "comm.hpp"
#include <dbg.hpp>

template<class T, typename... Args>
using has_foo_t = decltype(std::declval<T>().foo(std::declval<Args>()...));

template<typename T, typename... Args>
using has_foo2 = wws::is_detected<has_foo_t, T, Args...>;

struct A {
	void foo()
	{

	}
	void foo(double a)
	{

	}
};

int main()
{
	std::cout << std::boolalpha;
	dbg(has_foo2< A >::value);
	dbg(has_foo2< A, int >::value);
	dbg(has_foo2< A, double >::value);
#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}