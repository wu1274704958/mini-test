#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"
using namespace token;
namespace fs = std::filesystem;

void no_trans(std::vector<Token>& ts);

int main(int argc,char **argv)
{
	if (argc == 1)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\Ö÷Ô×´«ÆæBT°æ_5535£¨°®È¤£©_5.6_201907172014\\res\\values\\styles.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}
		no_trans(ts.tokens);
		auto out = f.generic_string();
		dbg(out);
		ts.save(out, true);
	}
	else
	{
		std::cout << "No find input file!" << std::endl;
		return -2;
	}

	return 0;
}

void no_trans(std::vector<Token>& ts)
{
	bool in_item = false;
	
	for (int i = 0;i < ts.size() - 1;++i)
	{
		auto& t = ts[i];
		if (t.body == "item" && t.per == '<')
		{
			in_item = true;
		}
		if (in_item && i > 0 && t.body == "item" && ts[i - 1].back == '/' && ts[i - 1].per == '<')
		{
			in_item = false;
		}

		if (in_item && t.body == "name" && t.back == '=')
		{
			if (ts[i + 1].body == "android:windowIsFloating" || ts[i + 1].body == "android:windowIsTranslucent")
			{
				ts[i + 3].body = "false";
				i += 3;
			}
		}
	}
}