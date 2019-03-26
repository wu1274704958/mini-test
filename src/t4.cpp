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

void play()
{
	test::Str s;
	//Str s2 = std::move(s);

	auto f = [s_ = std::forward<test::Str&&>(s),a = 2,&s]() {
		dbg(a);
		dbg(s_);
		dbg(s);
		std::cout << s_ <<std::endl;
	};
	dbg(getchar());
	dbg(1, 2, 1, 8, 5, 7);
	f();
}

int main()
{
	play();
#ifdef WIN32
	system("pause");
#endif 
	return 0;
}

