#pragma once

#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"

using namespace token;
namespace fs = std::filesystem;

void set_shortEdges(std::vector<Token>& ts);

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out_pkg\\res\\values\\styles.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		/*for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}*/
		set_shortEdges( ts.tokens);
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

void set_shortEdges(std::vector<Token>& ts)
{
	bool in_NoShadow = false;
	bool in_style = false;

	auto iter = ts.begin();

	for (int i = 0; i < ts.size(); ++i)
	{
		auto& it = ts[i];
		if (it.body == "style" && it.per == '<')
		{
			in_style = true;
		}
		if (in_style && it.body == "style" && it.back == '>')
		{
			in_style = false;
		}
		if (in_style && it.body == "name" && ts[i + 1].per == '"' && ts[i + 1].back == '"' && ts[i + 1].body == "Theme.NoShadow")
		{
			in_NoShadow = true;
		}
		if (in_NoShadow && it.back == '>')
		{
			++iter;
			break;
		}
		++iter;
	}

	if (iter != ts.begin())
	{
		Token s("<item name=\"android:windowLayoutInDisplayCutoutMode\">shortEdges</item>",'\n',Token::None);

		ts.insert(iter, std::move(s));
	}

}

