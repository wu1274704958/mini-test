#include "dbg.hpp"
#include "token_stream.hpp"
#include <filesystem>
using namespace token;
namespace fs = std::filesystem;

std::string get_pkg_name(std::vector<Token>& ts);
std::string rm_entry(const std::string& name, std::vector<Token>& ts);
void set_entry(std::string& s, const char* name, std::vector<Token>& ts);
void set_on_splash_destroy(const std::string& name, std::vector<Token>& ts);

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "No input path!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]); 
	fs::path f2(argv[2]);
	//fs::path f("C:\\Users\\admin\\Desktop\\qk_sub_pass\\out_pkg\\AndroidManifest.xml");
	//fs::path f2("C:\\Users\\admin\\Desktop\\temp_out_apk\\out_pkg2\\smali\\ane\\com\\nativelib\\SplashActivity.smali");
	if (fs::exists(f) && fs::exists(f2))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		
		std::string pkg_name = get_pkg_name(ts.tokens);
		pkg_name += ".AppEntry";
		
		std::cout << pkg_name << std::endl;

		std::string entry_s = rm_entry(pkg_name, ts.tokens);

		std::cout << entry_s << std::endl;

		set_entry(entry_s, "ane.com.nativelib.SplashActivity", ts.tokens);
		auto s_name = f.generic_string();
		ts.save(s_name,true);

		std::ifstream is2(f2.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts2(std::move(is2));
		ts2.analyse();

		set_on_splash_destroy(pkg_name, ts2.tokens);

		auto s_name2 = f2.generic_string();
		ts2.save(s_name2, true);
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

std::string rm_entry(const std::string& name, std::vector<Token>& ts)
{
	int in_activity_s = 0;
	int b = -1,e = -1;
	std::string res;
	for (int i = 0; i < ts.size(); ++i)
	{
		auto& t = ts[i];
		if (t.body == "activity" && t.per == '<')
			++in_activity_s;
		if (in_activity_s == 1 && t.body == "name")
		{
			if(ts[i + 1].body == name){
				++in_activity_s;
			}
			else {
				--in_activity_s;
			}
		}
		if (in_activity_s == 2 && t.body == "intent" && t.per == '<' && t.back == '-')
		{
			b = i; ++in_activity_s;
		}
		if (in_activity_s == 3 && t.body == "filter" && ts[i - 2].back == '/' && ts[i - 2].per == '<' && t.back == '>')
		{
			e = i + 1;
			break;
		}
	}

	if (b >= 0 && e >= 0)
	{
		for (int i = b; i <= e; ++i)
		{
			if (ts[i].per != Token::None)
				res += ts[i].per;
			if (!ts[i].body.empty())
				res += ts[i].body;
			if (ts[i].back != Token::None)
				res += ts[i].back;
		}

		ts.erase(ts.begin() + b, ts.begin() + e);
	}
	return res;
}

void set_entry(std::string& s, const char* name, std::vector<Token>& ts)
{
	int in_activity_s = 0;
	int b = -1;
	bool append_back = false;
	for (int i = 0; i < ts.size(); ++i)
	{
		auto& t = ts[i];
		if (t.body == "activity" && t.per == '<')
			++in_activity_s;
		if (in_activity_s == 1 && t.body == "name")
		{
			if (ts[i + 1].body == name) {
				++in_activity_s;
			}
			else {
				--in_activity_s;
			}
		}
		if (in_activity_s == 2 && t.back == '>')
		{
			if (t.per == '/')
			{
				t.per = Token::None;
				append_back = true;
			}
			b = i + 1;
			break;
		}
	}
	if (b >= 0)
	{
		if (append_back)
			s += "</activity>";
		ts.insert(ts.begin() + b, Token(s,'\n',Token::None));
	}

}

void set_on_splash_destroy(const std::string& name, std::vector<Token>& ts)
{
	bool in_constructor = false;
	for (int i = 0; i < ts.size(); ++i)
	{
		auto& t = ts[i];
		if (t.per == '.' && t.body == "method" && ts[i + 1].body == "static" && ts[i + 2].body == "constructor")
		{
			in_constructor = true;
		}
		if (in_constructor && t.body == "^_^" && t.per == '"' && t.back == '"')
		{
			t.body = name;
			break;
		}
	}
}