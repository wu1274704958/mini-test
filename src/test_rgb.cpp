#include "make_color.hpp"

struct Color {
	float r;
	float g;
	float b;
	float a;
};

int main()
{
	dbg((int)wws::FormHex<'A','f'>::val);
	dbg(0xaf);
	
	auto str = PREPARE_STRING("aa");
	dbg(str.get<0>());
	dbg(str.get<1>());
	dbg(str.get<2>());
	dbg(str.get<100>());
	dbg(str.len());
	dbg(wws::has_r_vt<std::string>::value);
	dbg(wws::has_r_vt<Color>::value);
	dbg(wws::has_g_vt<Color>::value);
	dbg(wws::has_b_vt<Color>::value);
	dbg(wws::has_a_vt<Color>::value);
	

	Color c = wws::make_rgba(PREPARE_STRING("1200ffff")).make<Color>();
	//auto c2 = wws::make_rgba(PREPARE_STRING("1200ffff")).make<std::string>(); // ���Զ������ This type must has member named r,g,b,a!!!
	dbg(std::make_tuple(c.r, c.g, c.b, c.a));

#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}
