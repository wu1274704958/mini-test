
#include <matrix2.hpp>

int main()
{
	using namespace wws;

	vec3 v;
	v.x() = 0.01f;
	dbg(v.x());
	dbg(v.y());

	vec3 v2({ 1.0f,2.0f,3.0f });
	//v2.x() = 0.01f;
	dbg(v2.x());
	dbg(v2.y());
	dbg(v2.z());

	return 0;
}