#include <iostream>
#include <stdio.h>
#include <utility>

using namespace std;

template<typename T>
struct can_sub_op1{
	template<typename U>
	static auto func(char) -> decltype( std::declval<U>().operator-(std::declval<U>()) , std::true_type() ) { return std::true_type(); }
	template<typename U> 
	static auto func(...) ->	std::false_type { return std::false_type(); }
	static const bool val = decltype( func<T>(0) )::value;
};

template<typename T>
struct can_sub_op2{
    template<typename U>
    static auto func(int) -> decltype( std::declval<U>() - std::declval<U>() , std::true_type() ) { return std::true_type(); }
    template<typename U>
    static auto func(...) ->    std::false_type { return std::false_type(); }
    static const bool val = decltype( func<T>(0) )::value;
};

template<typename T>
struct can_sub{
    static const bool val = can_sub_op1<T>::val || can_sub_op2<T>::val;
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
	friend std::ostream& operator<<<>(std::ostream& ,vec2<T>&& );
    friend std::ostream& operator<<<>(std::ostream& ,vec2<T>& );

};




int main()
{
	
	cout << can_sub<int>::val << endl;
	cout << sub(6,7) << endl;
	vec2 v1(2.0f,3.0f);
    vec2 v2(3.0f,4.0f);
    cout << v1 - v2 << endl;
    cout << sub(v2,v1) << endl;
	cout << v1 <<endl;
	return 0;
}
