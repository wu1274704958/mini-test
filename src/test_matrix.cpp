
#include <matrix2.hpp>
#include <make_color.hpp>

int main()
{
	using namespace wws;

	vec3 v;
	v.x() = 0.01f;
	dbg(v.x());
	dbg(v.y());
	dbg(v.b());

	vec3 v2({ 1.0f,2.0f,3.0f });
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

	return 0;
}