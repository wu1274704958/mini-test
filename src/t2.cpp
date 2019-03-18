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


template<typename T>
auto dbg_func(const char *expr,T&& t) -> T
{
	if constexpr(std::is_same<void,typename std::remove_cv<T>::type>::value)
	{
		std::cout << expr << " = void" << std::endl;
	}else{
		std::cout << expr << " = "<< t << std::endl;
	}
	return std::forward<T>(t);
}


template<typename T = void>
void dbg_func(const char *expr)
{
	std::cout << expr << " = void" << std::endl;
}

#define dbg(expr) dbg_func(#expr,expr)
	

void f()
{

}

int main()
{
	cout << std::boolalpha;
	//dbg( can_sub_op1<vec2<float >>::val );
	dbg_func<void>("aaa");
	cout << is_same<can_sub_op1<vec2<float >>::type,vec2<float >>::value <<endl;

    cout << can_sub_op1<int>::val << endl;
    cout << is_same<can_sub_op1<int>::type,int>::value <<endl;

	cout << can_sub<int>::val << endl;
	cout << is_same< int, can_sub<int>::type >::value << endl;

    cout << can_sub< vec2<double > >::val << endl;
    cout << is_same< vec2<double > , can_sub< vec2<double > >::type >::value << endl;
	cout << "test_sub: \n";
	cout << can_sub< test_sub >::val << endl;
	cout << is_same< char , can_sub< test_sub >::type >::value << endl;

	cout << sub(6,7) << endl;
	vec2 v1(2.0f,3.0f);
    vec2 v2(3.0f,4.0f);
    cout << v1 - v2 << endl;
    cout << sub(v2,v1) << endl;
	cout << v1 <<endl;
	pause();
	return 0;
}
