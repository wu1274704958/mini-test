
#include <matrix2.hpp>
#include <make_color.hpp>
#include <const_val.hpp>
#include <cgm_funs.hpp>

int main()
{
	using namespace cgm;
	using namespace const_val;

	vec3 v;
	v.x() = 0.01f;
	dbg(v.x());
	dbg(v.y());
	dbg(v.b());

	vec3 v2{ 1.0f,2.0f,3.0f };
	//v2.x() = 0.01f;
	dbg(v2);

	vec3 v3(1.0f);
	dbg(v2 + v3);
	dbg(-v2);
	dbg(v2 - v3);
	vec3 v4 = wws::make_rgb(PREPARE_STRING("#ff0897")).make<vec3>();
    dbg(v4);
	dbg(sizeof(vec3));
	dbg(vec2({3.0f,4.0f}).len());
	dbg(v3.len());
	dbg(v3.unitized());
    dbg(v3.unitized().len());
    dbg(vec2{1.0f,1.0f}.len());
    dbg(vec2{1.0f,1.0f}.unitized());
    dbg(zero<float>());
    dbg(pi<float>());
    dbg(pi<double >());
    dbg(pi_1_2<double >());
    dbg(v2 * v3);
    dbg(vec2{2.0f,2.0f} * vec2{3.0f,4.0f});

    dbg(radians(90.f));
    dbg(std::make_tuple(degrees(3.1415f),degrees(6.2830)));

    dbg(angle(vec2{1.0f,1.0f},vec2{-1.0f,1.0f}));
    dbg(cross(vec3{0.f,0.f,1.f},vec3{1.f,0.f,0.f}));
	return 0;
}