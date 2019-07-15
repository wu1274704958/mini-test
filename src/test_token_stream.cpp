#include "token_stream.hpp"
#include "dbg.hpp"
#include "print_stl.hpp"
using namespace token;

int main()
{
	Token t;
	dbg(t);
	Token t1("abc");
	dbg(t1);
	Token t2("wws", '<', '>');
	dbg(t2);
	Token t3("wws", Token::None, '>');
	dbg(t3);

	TokenStream<std::string> ts("var f = function(){ console.log(a); }");
	ts.analyse();
	for (auto &s : ts.tokens)
	{
		std::cout << s << "\n";
	}
	

#ifdef WIN32
	system("pause");
#endif
	return 0;
}