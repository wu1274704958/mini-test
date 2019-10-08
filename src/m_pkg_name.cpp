#include "dbg.hpp"
#include "token_stream.hpp"
#include <filesystem>
using namespace token;
namespace fs = std::filesystem;

void modify_pkg_name(std::vector<Token>& ts);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cout << "No input path!" << std::endl;
		return -1;
	}
	
	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\qk_sub_pass\\cqonline\\AndroidManifest.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		/*for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}*/
		modify_pkg_name(ts.tokens);
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

void modify_pkg_name(std::vector<Token>& ts)
{
	bool in_manifest = false;
	bool catched = false;
	bool in_activity = false;
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
				dbg(pkg_name);
			}
		}

		if (in_activity && t.back == '>')
			return;

		if (catched && t.body == "activity" && t.per == '<')
		{
			in_activity = true;
		}
		
		if (in_activity && t.body == "name" && t.back == '=')
		{
			ts[i + 1].body.insert(0, pkg_name);
			dbg(ts[i + 1].body);
			//return;
		}

		if (in_activity && t.body == "theme" && t.back == '=')
		{
			ts[i + 1].body = "@android:style/Theme.NoTitleBar.Fullscreen";
			dbg(ts[i + 1].body);
		}
	}
}