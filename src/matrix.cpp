#include <iostream>
#include "dbg.hpp"
#include <tuple>
//#include <type_traits>
#include <math.h>

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

	std::ostream& operator<<(std::ostream& out, vec2 &v)
	{
		out << "[" << v.x << " , " << v.y << "]";
		return out;
	}

	template<>
	union vec<float, 3>
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float r;
			float g;
			float b;
		};
		struct
		{
			float m1;
			float m2;
			float m3;
		};

		vec(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		vec()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}
		bool is_zero()
		{
			return x == 0.0f && y == 0.0f && z == 0.0f;
		}
		float len()
		{
			return sqrtf(powf(x,2.0f) + powf(y, 2.0f) + powf(z, 2.0f));
		}
		vec<float,3> normal()
		{
			if (this->is_zero())
				return vec<float, 3>();
			float l = this->len();
			return vec<float, 3>(x / l, y / l, z / l);
		}
	};
	typedef vec<float, 3> vec3;

	vec3 operator*(float k, vec3 oth)
	{
		return vec3(oth.x * k, oth.y * k,oth.z * k);
	}

	std::ostream& operator<<(std::ostream& out, vec3 &v)
	{
		out << "[" << v.x << " , " << v.y << " , " << v.z << "]";
		return out;
	}

	template<typename T, typename = std::enable_if_t< std::is_floating_point_v<T>>>
	T angle(T t)
	{
		return (t / 180.0) * 3.1415926535897932;
	}

	struct mat2 {
		float m11,m12,m21,m22;
		mat2()
		{
			m11 = 1.0f;
			m12 = 0.0f;
			m21 = 0.0f;
			m22 = 1.0f;
		}
		mat2(float m11_,float m12_,float m21_,float m22_):
					m11(m11_),
					m12(m12_),
					m21(m21_),
					m22(m22_)
		{}

		vec2 operator*(vec2 v)
		{
			return vec2(  m11 * v.x + m12 * v.y , m21 * v.x + m22 * v.y  );
		}

		static mat2 form_angle(float angle)
		{
			return mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
		}
	};

	std::ostream& operator<<(std::ostream& out, mat2 &m)
	{
		out << "\n[" << m.m11 <<" , "<< m.m12 << "]\n";
		out << '[' << m.m21 <<" , "<< m.m22 << ']';
		return out;
	}


	struct mat3 {
		float	m11, m12, m13, 
				m21, m22, m23,
				m31, m32 ,m33;
		mat3()
		{
			m11 = 1.0f;m12 = 0.0f;m13 = 0.0f;
			m21 = 0.0f;m22 = 1.0f;m23 = 0.0f;
			m31 = 0.0f;m32 = 0.0f;m33 = 1.0f;
		}
		mat3(	float m11_, float m12_, float m13_, 
				float m21_, float m22_, float m23_,
				float m31_, float m32_, float m33_) :
			m11(m11_),m12(m12_),m13(m13_),
			m21(m21_),m22(m22_),m23(m23_),
			m31(m31_),m32(m32_),m33(m33_)
		{}

		vec3 operator*(vec3 v)
		{
			return vec3(m11 * v.x + m12 * v.y + m13 * v.z, m21 * v.x + m22 * v.y + m23 * v.z, m31 * v.x + m32 * v.y + m33 * v.z);
		}

		mat3 operator*(mat3 v)
		{
			return mat3(m11 * v.m11 + m12 * v.m21 + m13 * v.m31, m11 * v.m21 + m12 * v.m22 + m13 * v.m23, m11 * v.m31 + m12 * v.m32 + m13 * v.m33,
						m21 * v.m11 + m22 * v.m21 + m23 * v.m31, m21 * v.m21 + m22 * v.m22 + m23 * v.m23, m21 * v.m31 + m22 * v.m32 + m23 * v.m33,
						m31 * v.m11 + m32 * v.m21 + m33 * v.m31, m31 * v.m21 + m32 * v.m22 + m33 * v.m23, m31 * v.m31 + m32 * v.m32 + m33 * v.m33);
		}

		static mat3 form_rotate_x(float angle)
		{
			return mat3(1.0f,0.0f,0.0f,
						0.0f,cos(angle),sin(angle),
						0.0f,-sin(angle),cos(angle));
		}
		static mat3 form_rotate_y(float angle)
		{
			return mat3(
				cos(angle),0.0f, -sin(angle),
				0.0f, 1.0f, 0.0f,
				sin(angle), 0.0f, cos(angle));
		}
		static mat3 form_rotate_z(float angle)
		{
			return mat3(
				cos(angle), sin(angle),0.0f ,
				-sin(angle), cos(angle),0.0f ,
				0.0f, 0.0f, 1.0f);
		}

		static mat3 form_rotate(vec3 n, float angle)
		{
			float cos_a = cos(angle);
			float sin_a = sin(angle);

			return mat3( pow(n.x,2.0f) * (1.0f - cos_a) + cos_a, n.x * n.y * (1.0f - cos_a) + n.z * sin_a,  n.x * n.z *(1.0f - cos_a) - n.y * sin_a,
						 n.x * n.y * (1.0f - cos_a) - n.z * sin_a, pow(n.y,2.0f) * (1.0f - cos_a) + cos_a , n.y * n.z * (1.0f - cos_a) + n.x * sin_a,
						 n.x * n.z * (1.0f - cos_a) + n.y * sin_a,	n.y * n.z * (1.0f - cos_a) - n.x * sin_a, pow(n.z,2.0f) * (1.0f - cos_a) + cos_a  );
		}
	};

	std::ostream& operator<<(std::ostream& out, mat3 &m)
	{
		out << "\n[" << m.m11 << " , " << m.m12 << " , "<< m.m13 <<  "]\n";
		out << '[' << m.m21 << " , " << m.m22 << " , " << m.m23 << "]\n";
		out << '[' << m.m31 << " , " << m.m32 << " , " << m.m33 << ']';
		return out;
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

	dbg(angle(180.0f));

	mat2 m1 = mat2::form_angle(angle(45.0f));

	dbg(m1 * vec2(1,1));

	mat2 m2 = mat2::form_angle(angle(135.0f));

	dbg(m2);

	mat2 m3;
	
	dbg(dbg(m3) * vec2(2, 8));
	
	mat3 m4 = mat3::form_rotate_x(angle(45.0f));
	dbg(m4 * vec3(1,1,1));

	dbg(vec3(1.0f, 2.0f, 1.0f).normal());

	vec3 v_123 = vec3(1.0f, 0.0f, 0.0f);
	dbg(mat3::form_rotate_z(angle(45.0f)) * mat3::form_rotate_y(angle(45.0f)) * v_123);

	dbg(mat3::form_rotate(v_123, angle(90.0f)) * vec3(1.0f, 1.0f, 1.0f));

#if defined(WIN32)
	system("pause");
#endif
	return 0;
}