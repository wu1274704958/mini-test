#pragma once

#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"

using namespace token;
namespace fs = std::filesystem;

void set_shortEdges(std::vector<Token>& ts);
void set_shortEdges2(std::vector<Token>& ts, std::string& pkg_name);
std::string get_pkg_name(std::vector<Token>& ts);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}
	fs::path f(argv[1]);
	std::string s2(argv[2]);
	//fs::path f2("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out_pkg\\smali\\one\\chuanqi\\online\\AppEntry.smali");
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

	std::string s3 = s2 + "AndroidManifest.xml";

	fs::path mainfest(s3);
	std::string pkg_name;
	if (fs::exists(mainfest))
	{
		std::ifstream is(mainfest.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		
		pkg_name = get_pkg_name(ts.tokens);
		
		for (auto& c : pkg_name)
		{
			if (c == '.')
				c = '/';
		}
		dbg(pkg_name);
	}
	else
	{
		std::cout << "No find input file!" << std::endl;
		return -2;
	}

	std::string s4 = s2 + "smali/";
	s4 += pkg_name;
	s4 += "/AppEntry.smali";

	dbg(s4);

	fs::path f2(s4);
	if (fs::exists(f2))
	{
		std::ifstream is(f2.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		/*for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}*/
		set_shortEdges2(ts.tokens,pkg_name);
		auto out = f2.generic_string();
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

void set_shortEdges2(std::vector<Token>& ts,std::string& pkg_name)
{
	bool in_onCreate = false;
	auto it1 = -1;
	int i_idx1 = -1;


	for (int i = 0; i < ts.size(); ++i)
	{
		auto& it = ts[i];
		if (it.per == '.' && it.body == "method" && ts[i + 1].body == "public" && ts[i + 2].body == "onCreate")
		{
			in_onCreate = true;
			i += 3;
			while (ts[i].body != "param") { ++i;  }
			while (ts[i].back != '\n') { ++i;  }
			++i;
			it1 = i;
			break;
		}
	}

	if (in_onCreate)
	{
		Token t(
			" sget v0, Landroid/os/Build$VERSION;->SDK_INT:I\n"
			" const/16 v1, 0x1c\n"
			" if-lt v0, v1, :cond_7\n"
			" invoke-virtual {p0}, Lone/chuanqi/online/AppEntry;->getWindow()Landroid/view/Window;\n"
			" move-result-object v0\n"
			" invoke-virtual {v0}, Landroid/view/Window;->getAttributes()Landroid/view/WindowManager$LayoutParams;\n"
			" move-result-object v0\n"
			" const/4 v1, 0x1\n"
			" iput v1, v0, Landroid/view/WindowManager$LayoutParams;->layoutInDisplayCutoutMode:I	\n"
			" invoke-virtual {p0}, Lone/chuanqi/online/AppEntry;->getWindow()Landroid/view/Window;\n"
			" move-result-object v2\n"
			" invoke-virtual {v2, v0}, Landroid/view/Window;->setAttributes(Landroid/view/WindowManager$LayoutParams;)V\n"
			, Token::None, '\n');

		for (int i = 0; i < t.body.size(); ++i)
		{
			if (t.body[i] == 'L' && t.body[i + 1] == 'o' && t.body[i + 2] == 'n' && t.body[i + 3] == 'e')
			{
				t.body.replace(i + 1, 18, pkg_name);
			}
		}

		dbg(t.body);

		ts.insert(ts.begin() + it1, std::move(t));
		++it1;
		while (ts[it1].body != "invoke" && ts[it1 + 1].body != "super")
		{
			++it1;
		}
		Token t2(":cond_7", ' ', '\n');
		ts.insert(ts.begin() + it1, std::move(t2));
	}


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
