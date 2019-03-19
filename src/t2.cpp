#include <iostream>
#include <stdio.h>
#include <utility>

using namespace std;

template <bool B,typename T>
struct has_type;

template <typename T>
struct has_type<true,T>{
    using type = T;
    static const bool val = true;
};

template <>
struct has_type<false,void>{
    using type = void;
    static const bool val = false;
};

template<typename T>
struct can_sub_op1{
	template<typename U>
	static auto func(char)
	-> has_type<true, decltype( std::declval<U>().operator-(std::declval<U>()) ) >
	{
	    return has_type< true, decltype( std::declval<U>().operator-(std::declval<U>()) )> {};
	}
	template<typename U> 
	static auto func(...) ->has_type<false,void> { return has_type<false,void>{}; }
	static const bool val = decltype( func<T>(0) )::val;
	using type = typename decltype( func<T>(0) )::type;
};

template<typename T>
struct can_sub_op2{
    template<typename U>
    static auto func(int)
    -> has_type< true, decltype( std::declval<U>() - std::declval<U>() )>
    {
        return has_type< true,  decltype( std::declval<U>() - std::declval<U>()) > {};
    }
    template<typename U>
    static auto func(...) -> has_type<false,void> { return has_type<false,void>{}; }
    static const bool val = decltype( func<T>(0) )::val;
    using type = typename decltype( func<T>(0) )::type;
};

template <bool B,typename T1,typename T2>
struct choose_if{
    using type = T2;
};

template <typename T1,typename T2>
struct choose_if<true,T1,T2>
{
    using type = T1;
};

template<typename T>
struct can_sub{
    static const bool val = can_sub_op1<T>::val || can_sub_op2<T>::val;
    using type = typename choose_if< can_sub_op1<T>::val , typename can_sub_op1<T>::type ,
                    typename choose_if< can_sub_op2<T>::val , typename can_sub_op2<T>::type , void >::type >::type;
};

template<typename T >
auto sub(T t1,T t2) -> std::enable_if_t< can_sub<T>::val , T >
{	
	return t1 - t2;
}

template<typename T>
class vec2;

template<typename T>
std::ostream& operator<<(std::ostream& out,vec2<T>&& v)
{
	out << "[" << v.x << ","<< v.y <<"]";
	return out;
}
template<typename T>
std::ostream& operator<<(std::ostream& out,vec2<T>& v)
{
	out << "[" << v.x << ","<< v.y <<"]";
	return out;
}
template<typename T>
class vec2{
private:
	T x;
	T y;
public:
	vec2(T x_,T y_) : x(x_) ,y(y_)
	{
		
	}
    vec2(const vec2<T>&) = default;
    vec2<T> operator-(vec2<T> v)
    {
        return vec2(x - v.x,y - v.y);
    }
	friend std::ostream& operator<<<T>(std::ostream& ,vec2<T>&& );
    friend std::ostream& operator<<<T>(std::ostream& ,vec2<T>& );

};

struct test_sub{
	char operator-(test_sub ts)
	{
		return 'A';
	}
};

void pause()
{
#if defined(WIN32)
		system("pause");
#endif
}


template <typename T>
struct Tv{
    using type = T;
    static const bool val = true;
    T t;
    Tv(T t_) : t(t_)
    {

    }
};

template <>
struct Tv<void>{
    using type = void;
    static const bool val = false;
    Tv(int i)
    {

    }
};

template<typename T>
auto dbg_func(const char *expr,Tv<T>&& tv) -> T
{
    if constexpr( Tv<T>::val )
    {
        std::cout << expr << " = "<< tv.t << std::endl;
        return std::forward<T>(tv.t);
    }else{
        std::cout << expr << " = void" << std::endl;
    }
}

template<typename ...T>
struct FirstArge;

template<typename T>
struct FirstArge<T>
{
	using type = T;
};

template<>
struct FirstArge<>
{
	using type = Tv<void>;
};

template<bool B,typename... V>
auto dbg_choose(const char *p, V&& ... t) -> typename FirstArge<V...>::type::type
{
	if constexpr (B)
	{
		return dbg_func(p, std::forward<V>(t) ...);
	}
	else {
		dbg_func(p, Tv<void>(1));
	}
}

#define dbg(expr,...)  dbg_choose< Tv<decltype(expr)>::val >(#expr, Tv<decltype(expr)>( (expr,__VA_ARGS__) ) )
	

void f()
{
	
}

int main()
{
	cout << std::boolalpha;
	if ( dbg( can_sub_op1<vec2<float >>::val ) )
    {
	    cout << "is true \n";
    }

	dbg(f(),1);
	//dbg_choose< !std::is_same_v<void, decltype(can_sub_op1<vec2<float >>::val)> ,Tv<const bool> >(" ssss", Tv<decltype(can_sub_op1<vec2<float >>::val)>(can_sub_op1<vec2<float >>::val));

	//dbg_choose< false >(" ssss");
	dbg( (std::is_same<can_sub_op1<vec2<float >>::type,vec2<float> >::value) );

	dbg((can_sub_op1<int>::val));
	dbg((is_same<can_sub_op1<int>::type,int>::value ));

	dbg((can_sub<int>::val));
	dbg((is_same< int, can_sub<int>::type >::value));

	dbg((can_sub< vec2<double > >::val));
	dbg((is_same< vec2<double >, can_sub< vec2<double > >::type >::value));
	cout << "test_sub: \n";
	dbg(( can_sub< test_sub >::val ));
	dbg(( is_same< char , can_sub< test_sub >::type >::value ));

	dbg(sub(6, 7));
	vec2 v1(2.0f,3.0f);
    vec2 v2(3.0f,4.0f);
    dbg( v1 - v2 );
    dbg( sub(v2,v1) );
	dbg( v1 );
	pause();
	return 0;
}
