#include <dbg.hpp>
#include <cstring>

struct Str{
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
		data = new char[10];
		memcpy(data,s.data,sizeof(char) * 10);
	}
	Str(Str && s)
	{
		dbg("Str(Str &&)");
		if(data)
		{
			delete [] data;
		}
		data = s.data;
		s.data = nullptr;
	}
	~Str()
	{
		dbg("~Str()");
		if(data)
			delete[] data;
	}
	friend std::ostream& operator<<(std::ostream& out,const Str &s);
};

std::ostream& operator<<(std::ostream& out,const Str &s)
{
	out << s.data;
	return out;
}

void play()
{
	Str s;
	//Str s2 = std::move(s);

	auto f = [s_ = std::forward<Str&&>(s)]() {
		dbg(s_);
		std::cout << s_ <<std::endl;
	};
	dbg(getchar());
	f();
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

