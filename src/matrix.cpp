#include <iostream>
#include "dbg.hpp"
#include <tuple>

namespace gm{
	template<typename Ty,std::size_t Dim>
	union vec {
		Ty a[Dim];
	};

	template<>
	union vec<float, 2>
	{
		struct 
		{
			float x;
			float y;
		};
		struct
		{
			float r;
			float g;
		};
		struct
		{
			float m1;
			float m2;
		};

		vec(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		vec()
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}
	};
	typedef vec<float, 2> vec2;

	vec2 operator*(float k, vec2 oth)
	{
		return vec2(oth.x * k, oth.y * k);
	}
}

using namespace gm;
int main()
{
	dbg(sizeof(gm::vec2));
	vec2 v = vec2(2.0f, 3.0f);
	dbg( std::make_tuple(v.r,v.g) );

	vec2 v2 = 2.0f * v;
	dbg(std::make_tuple(v2.x, v2.y));

	dbg(std::make_tuple(1, 2, 34, 56, 87, 100));

#if defined(WIN32)
	system("pause");
#endif
	return 0;
}