#include <dbg.hpp>
#include <cstring>
namespace test {
	struct Str {
	private:
		char *data = nullptr;
	public:
		Str()
		{
			data = new char[10];
			memset(data, 0, sizeof(char) * 10);
			data[0] = 'a';
			data[1] = 'b';
			data[2] = 'c';
			dbg(data);
		}
		Str(const Str& s)
		{
			dbg("const Str&");
			if (s.data)
			{
				data = new char[10];
				memcpy(data, s.data, sizeof(char) * 10);
			}
		}
		void set(int i,char c)
        {
		    if(data)
		        data[i] = c;
        }
		Str(Str && s)
		{
			dbg("Str(Str &&)");
			if (data)
			{
				delete[] data;
			}
			data = s.data;
			s.data = nullptr;
		}
		~Str()
		{
			dbg("~Str()");
			if (data)
				delete[] data;
		}
		friend std::ostream& operator<<(std::ostream& out, const Str &s);
	};

	std::ostream& operator<<(std::ostream& out, const Str &s)
	{
		if (s.data)
			out << s.data;
		else
			out << "null";
		return out;
	}
}

struct Y{
    template<typename T>
    static T a;
}; 

void play()
{
	test::Str s;
	//Str s2 = std::move(s);

	auto f = [s_ = std::move(s),a = 2,&s]() mutable { //不加 mutable s_的类型是 const test::Str, a的类型是 const int,s的类型是test::Str&
		dbg(a);
		dbg(s_);
		s_.set(0,'b');
		dbg(s_);
		dbg(s);
		std::cout << s_ <<std::endl;
	};
	dbg(getchar());
	dbg(1, 2, 1, 8, 5, 7);
	f();
}


template<> int Y::a<int> = 6;
template<> char Y::a<char> = 'b';
template<> test::Str Y::a<test::Str> = test::Str();


int main()
{
	dbg(sizeof(Y::a<char>));
    dbg(Y::a<int>);
	dbg(Y::a<test::Str>);
	play();
#ifdef WIN32
	system("pause");
#else
    printf("after.play()\n");
#endif 
	return 0;
}

