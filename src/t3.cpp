#include <dbg.hpp>

struct MyStruct
{
	int a;
	int c;
	static int b;
	MyStruct()
	{
		dbg("run MyStruct");
	}
	int f() { dbg("run f"); return 1; }
	static int f2() { dbg("run f2"); return 2; }
};

int MyStruct::b = 2;

int main()
{
	using type = decltype(&MyStruct::a);
	using type2 = decltype(&MyStruct::b);
	std::cout << std::boolalpha;
	dbg((std::is_same_v< int (MyStruct::*)(), decltype(&MyStruct::f)> ));
	dbg((std::is_same_v< int(*)(), decltype(&MyStruct::f2)>));
	auto p_of_a = dbg(&MyStruct::a);
	auto p_of_b = dbg(&MyStruct::b);
	auto p_of_c = dbg(&MyStruct::c);

	MyStruct ms = dbg(MyStruct());

	dbg(ms.*p_of_a);
	dbg(*p_of_b);
	dbg(ms.*p_of_c);

	auto p_of_f = dbg(&MyStruct::f);
	auto p_of_f2 = dbg(&MyStruct::f2);

	dbg( (ms.*p_of_f)() );
	dbg(p_of_f2());

#ifdef  WIN32
	system("pause");
#endif //  WIN32

	return 0;
}