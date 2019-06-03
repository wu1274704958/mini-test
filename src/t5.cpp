#include <gray.hpp>
#include <dbg.hpp>


#define def_with_const_str(name,...)  \
__VA_ARGS__								\
namespace defined_const_str{			\
	const char * name  = #__VA_ARGS__;	\
};	

def_with_const_str(Test,struct Test{
	int a = 1;
	char b = 2;
	int c = 3;
};)

def_with_const_str(Test2, struct Test2{
	int a = 1;
	char b = 2;
	short c = 3;
};)

def_with_const_str(Test3,struct Test3{
	int a = 1;
	char b = 2;
	char c = 3;
	char d = 4;
};)

int hhhh()
{
	return 3;
}

struct Go{
	using go = int(*)();
	operator go()
	{
		return hhhh;
	}
};

class father{
public:
    int a = 1;
//    virtual void run()
//    {
//
//    }
};

class mother{
public:
    int b = 2;
};

class son : public father,public mother{
public:
    int c = 3;

//    void run() override {
//
//    }
};

int main()
{
	dbg(defined_const_str::Test);
	dbg(defined_const_str::Test2);
	dbg(defined_const_str::Test3);
	Test t;
	dbg(sizeof(t));
	int *p = reinterpret_cast<int*>(&t);
	dbg(t.a);
	dbg(t.b);
	dbg(t.c);
	printBin(*p);
	printBin(*(p + 1));
	printBin(*(p + 2));

	Test2 t2;
	dbg(t2.a);
	dbg(t2.b);
	dbg(t2.c);
	dbg(sizeof(t2));
	p = reinterpret_cast<int *>(&t2);
	printBin(*p);
	printBin(*(p + 1));
	
	Test3 t3;
	dbg(t3.a);
	dbg(t3.b);
	dbg(t3.c);
	dbg(t3.d);
	dbg(sizeof(t3));
	p = reinterpret_cast<int *>(&t3);
	printBin(*p);
	printBin(*(p + 1));
	Go g;
	dbg(g.operator Go::go()());
//---------------------------------多重继承C style强转
	son* s = new son();
    dbg(std::make_tuple(s,s->a,s->b,s->c)); //没有虚函数时可能的输出
	father* f = (father*)(s);   //f = 000001C10C202550
    dbg(f);
    mother* m = (mother*)(s);   //m = 000001C10C202554
    dbg(m);
    dbg(sizeof(son));
    printBin(s);                //0000 0000 0000 0000 0000 0000 0000 0001
    delete s;                   //0000 0000 0000 0000 0000 0000 0000 0010
                                //0000 0000 0000 0000 0000 0000 0000 0011
#ifdef WIN32
	system("pause");
#endif
	return 0;
}
