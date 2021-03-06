
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
    dbg(v2 * 2.0f);
    dbg(vec<double ,3>{3.0,4.0,5.0} * 3.0 );

    mat2 m1 (1.0f);
    mat2 m2 { 1.0f , 2.0f,
              3.0f , 4.0f};
    mat4 m3 (1.0f);

    dbg(m1);
    dbg(m2);
    dbg(m3);

    dbg(std::make_tuple( m2.m11(),m2.m12(),m2.m21(),m2.m22() ));

    matrix<float ,2,3> m4{
        1.0f,2.0f,3.0f,
        4.0f,5.0f,6.0f
    };

    dbg(m4);
    dbg(m4.transpose());
    dbg(m4.m13());
    dbg(m4.m23());

    matrix<float ,3,2> m5{
            1.0f,2.0f,
            3.0f,4.0f,
            5.0f,6.0f
    };

    dbg(m5);
    dbg(m5.transpose());
    dbg(m5.m31());
    dbg(m5.m32());

    mat3 m6 { 1.f,2.f,3.f,
              4.f,5.f,6.f,
              7.f,8.f,9.f};
    dbg(m6);
    dbg(m6.transpose());

	dbg(2.0f * m6);
	dbg(3.0f * m5);

	dbg(m5 * vec2{ 1.0f,1.0f });

	dbg(m4 * m5);

	dbg(cofactor(m6, 1, 1));

	mat4 m7{1.0f,2.0f,3.0f,4.0f,
			5.0f,6.0f,7.0f,8.0f,
			9.0f,8.0f,7.0f,6.0f,
			5.0f,4.0f,3.0f,2.0f};
	dbg(cofactor(m7, 2, 3));
	dbg(cofactor(m7, 4, 4));

	dbg(det(m2));
	dbg(det(m6));
	auto m6_tp = m6.transpose();
	dbg(det(m6_tp));
	mat3 m8(1.0f);
	dbg(det(m8));
	m8.m12() = 0.1f;
	m8.m21() = -0.1f;
	m8.m32() = 0.1f;
	dbg(m8);
	dbg(det(m8));

	mat3 m9{ -4.f,-3.f,3.f,
			0.f,2.f,-2.f,
			1.f,4.f,-1.f };

	dbg(adj(m9));

	mat3 m10{
		6.f,9.f,0.f,
		-2.0f,1.f,-8.f,
		-2.f,13.f,-8.f
	};

	mat3 m11{
		1,2,4,
		3,6,9,
		-2,7,9
	};

	dbg(det(m10));
	dbg(inv(m9));
	dbg(inv(m10));
	dbg(inv(m11));
	return 0;
}