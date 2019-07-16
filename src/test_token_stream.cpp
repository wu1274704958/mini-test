#include "token_stream.hpp"
#include "dbg.hpp"
#include "print_stl.hpp"
#include <filesystem>
#include <fstream>
using namespace token;

int main(int argc,char **argv)
{
	Token t;
	dbg(t);
	Token t1("abc");
	dbg(t1);
	Token t2("wws", '<', '>');
	dbg(t2);
	Token t3("wws", Token::None, '>');
	dbg(t3);

	if (argc <= 1)
	{
		TokenStream<std::string> ts("var a = 30;\n"
		"	var b = function()\n"
		"{\n"
		"	alert(\"hello\" + a);\n"
		"}\n"
		"\n"
		"b(); ");
		ts.analyse();
		for (auto &s : ts.tokens)
		{
			std::cout << s << "\n";
		}
	}
	else
	{
		namespace fs = std::filesystem;
		fs::path f(argv[1]);
		//fs::path f("C:/Users/admin/Desktop/test.js");
		if (fs::exists(f))
		{
			auto path = f.generic_string();
			dbg(path);
			std::ifstream ifs(path.data(), std::ios::binary);
			/*while (!ifs.eof())
			{
				char a;
				ifs.get(a);
				std::cout << a;
			}*/
			TokenStream<std::ifstream> ts(std::move(ifs));
			ts.analyse();
			dbg(ts.tokens.size());
			for (auto &s : ts.tokens)
			{
				std::cout << s << "\n";
			}
			std::string out = "C:/Users/admin/Desktop/test2.js";
			ts.save(out,true);
		}
	}
	

#ifdef WIN32
	system("pause");
#endif
	return 0;
}