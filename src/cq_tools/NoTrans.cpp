#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"
#include "print_stl.hpp"
#include <algorithm>

using namespace token;
namespace fs = std::filesystem;

void no_trans(std::vector<Token>& , std::vector<std::string>&);
std::vector<std::string> no_trans_am(std::vector<Token>&);

int main(int argc,char **argv)
{
	if (argc <= 2)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}

	fs::path am_path(argv[1]);
	//fs::path am_path("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out\\AndroidManifest.xml");
	std::vector<std::string> used_style_names;
	if (fs::exists(am_path))
	{
		std::ifstream is(am_path.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}
		used_style_names = no_trans_am(ts.tokens);
		auto out = am_path.generic_string();
		dbg(out);
		ts.save(out, true);
	}
	else
	{
		std::cout << "No find input file!" << std::endl;
		return -2;
	}
	
	dbg(used_style_names);

	fs::path style_path(argv[2]);
	//fs::path style_path("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out\\res\\values\\styles.xml");
	if (fs::exists(style_path) && !used_style_names.empty())
	{
		std::ifstream is(style_path.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}
		no_trans(ts.tokens,used_style_names);
		auto out = style_path.generic_string();
		dbg(out);
		ts.save(out, true);
	}

	return 0;
}

std::vector<std::string> no_trans_am(std::vector<Token>& ts)
{
	std::vector<std::string> used_style_names;
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
			else {
				TokenStream<std::string> temp_t(ts[i + 1].body);
				temp_t.analyse();
				if (temp_t.tokens[0].body == "@style" && temp_t.tokens[0].back == '/')
				{
					std::string t_res;
					for (int i = 1; i < temp_t.tokens.size(); ++i)
					{
						if(temp_t.tokens[i].per != Token::None)
							t_res += temp_t.tokens[i].per;
						t_res += temp_t.tokens[i].body;
						if (temp_t.tokens[i].back != Token::None)
							t_res += temp_t.tokens[i].back;
					}
					used_style_names.push_back(t_res);
				}
			}
		}
	}
	return used_style_names;
}

void no_trans(std::vector<Token>& ts, std::vector<std::string>& used_style_names)
{
	bool in_item = false;
	bool in_style = false;
	bool in_used_style = false;
	for (int i = 0;i < ts.size() - 1;++i)
	{
		auto& t = ts[i];
		
		if (t.body == "style" && t.per == '<')
		{
			in_style = true;
		}

		if (in_style && i > 0 && t.body == "style" && ts[i - 1].back == '/' && ts[i - 1].per == '<')
		{
			in_style = false;
			in_used_style = false;
		}
		if (in_style && t.body == "name" && t.back == '=' && (std::find(std::begin(used_style_names), std::end(used_style_names), ts[i + 1].body) != std::end(used_style_names)))
		{
			in_used_style = true;
		}

		if (in_used_style && t.body == "item" && t.per == '<')
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