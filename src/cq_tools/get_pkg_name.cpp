#include "dbg.hpp"
#include "token_stream.hpp"
#include <filesystem>
using namespace token;
namespace fs = std::filesystem;

std::string get_pkg_name(std::vector<Token>& ts);

int main(int argc, char** argv)
{
	char p = -1;
	if (argc == 1)
	{
		std::cout << "No input path!" << std::endl;
		return -1;
	}
	if (argc > 2)
		p = argv[2][0];

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out_pkg\\AndroidManifest.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		/*for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}*/
		std::string pkg_name = get_pkg_name(ts.tokens);
		if (p >= 0)
		{
			for (auto& c : pkg_name)
			{
				if (c == '.')
					c = p;
			}
		}
		std::cout << pkg_name << std::endl;
	}
	else
	{
		std::cout << "No find input file!" << std::endl;
		return -2;
	}

	return 0;
}

std::string get_pkg_name(std::vector<Token>& ts)
{
	bool in_manifest = false;
	bool catched = false;
	std::string pkg_name;
	for (int i = 0; i < ts.size(); ++i)
	{
		auto& t = ts[i];
		if (t.body == "manifest" && t.per == '<')
		{
			in_manifest = true;
		}
		if (in_manifest && t.back == '>')
		{
			in_manifest = false;
		}

		if (in_manifest && t.body == "package" && t.back == '=')
		{
			if (ts[i + 1].per == '"' && ts[i + 1].back == '"')
			{
				pkg_name = ts[i + 1].body;
				catched = true;
				//dbg(pkg_name);
			}
		}
	}
	return pkg_name;
}