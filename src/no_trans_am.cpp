#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"
using namespace token;
namespace fs = std::filesystem;

void no_trans_am(std::vector<Token>& ts);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out\\AndroidManifest.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}
		no_trans_am(ts.tokens);
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

void no_trans_am(std::vector<Token>& ts)
{
	bool in_activity = false;
	//bool is_login = false;
	for (int i = 0; i < ts.size() - 1; ++i)
	{
		auto& t = ts[i];
		if (t.body == "activity" && t.per == '<')
		{
			in_activity = true;
		}
		if (in_activity && t.back == '>')
		{
			in_activity = false;
			//is_login = false;
		}

		/*if (in_activity && t.body == "android:name" && t.back == '=')
		{
			is_login = ts[i + 1].body.find("Login") != std::string::npos;
		}*/

		if (in_activity && t.body == "theme" && t.back == '=')
		{
			bool temp = ts[i + 1].body.find("@android:style") != std::string::npos;
			if (temp) //
			{
				std::string::size_type s;
				if ((s = ts[i + 1].body.find("Translucent")) != std::string::npos)
				{
					ts[i + 1].body.erase(s, 12);
					if (ts[i + 1].body.back() == '.')
					{
						ts[i + 1].body.append("NoTitleBar.Fullscreen");
					}
				}
			}
		}
	}
}