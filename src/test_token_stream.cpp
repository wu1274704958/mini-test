#include "token_stream.hpp"
#include "dbg.hpp"
#include "print_stl.hpp"
#include <filesystem>
#include <fstream>
using namespace token;

template<typename T>
void confound_js(TokenStream<T>& ts);

void confound_as(std::vector<Token>& ts);

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
		dbg("analyse");
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
		//fs::path f("C:\\Users\\wushu\\Desktop\\HallView.as");
		int is_support = 0;
		if (f.has_extension())
		{
			auto ext = f.extension();
			dbg(ext.generic_string());
			if (ext == ".js" || ext == ".html")
				is_support = 1;
			if (ext == ".as")
				is_support = 2;
			dbg(is_support);
		}
		
		
		if (fs::exists(f) && is_support > 0)
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
			switch (is_support)
			{
			case 1:
				confound_js(ts);
				break;
			case 2:
				confound_as(ts.tokens);
				break;
			default:
				break;
			}

			fs::path fn = f.filename();
			std::string nn = fn.generic_string();
			int i = nn.find_last_of('.');
			nn.insert(i, 1, '2');
			fs::path out = f.replace_filename(fs::path(nn));
			std::string out_path = out.generic_string();
			ts.save(out_path,true);
		}
	}
	

#ifdef WIN32
	system("pause");
#endif
	return 0;
}

#include <unordered_map>

std::string get_under_line(int n)
{
	std::string str;
	str.reserve(n);
	for (int i = 0; i < n; ++i)
		str += '_';
	return str;
}

template<typename T>
void confound_js<T>(TokenStream<T>& ts)
{
	int line_n = 3,l_line_n = 1;
	std::unordered_map<std::string, int> g_map;
	std::unordered_map<std::string, int> l_map;
	int if_stage = 0;
	int deep = 0;
	for (int i = 0;i < ts.tokens.size();++i)
	{
		auto& s = ts.tokens;
		auto& curr = s[i];

	
		if (if_stage == 0 && curr.body == "function")
			if_stage = 1;
		
		if (curr.per == '(' && if_stage == 1)
		{
			l_line_n = line_n + 1;
			while (s[i].back != ')')
			{
				if (s[i].body != "")
				{
					l_map[s[i].body] = l_line_n;
					s[i].body = get_under_line(l_line_n);
					++l_line_n;
				}
				++i;
			}
			if (s[i].body != "" && s[i].back == ')')
			{
				l_map[s[i].body] = l_line_n;
				s[i].body = get_under_line(l_line_n);
				++l_line_n;
			}
			if_stage = 2;
		}

		if (curr.per == '{' && if_stage == 2)
		{
			if_stage = 3;
		}
		if (curr.per == '{' && if_stage == 3)
		{
			++deep;
		}

		if (curr.back == '}' && if_stage == 3)
		{
			--deep;
			if (deep == 0)
			{
				if_stage = 0;
				l_map.clear();
				l_line_n = 1;
			}
		}

		if (curr.body == "var")
		{
			if (if_stage == 0)
			{
				g_map[s[i+1].body] = line_n;
				++line_n;
			}
			else {
				l_map[s[i + 1].body] = l_line_n;
				++l_line_n;
			}
			continue;
		}
		for (auto it = g_map.begin(); it != g_map.end(); ++it)
		{
			if (it->first == curr.body && curr.per != '"' && curr.back != '"')
			{
				curr.body = get_under_line(it->second);
			}
		}

		if (if_stage == 3)
		{
			auto & curr_local_map = l_map;
			for (auto it = curr_local_map.begin(); it != curr_local_map.end(); ++it)
			{
				if (it->first == curr.body && curr.per != '"' && curr.back != '"')
				{
					curr.body = get_under_line(it->second);
				}
			}
		}
	}
}


void confound_as(std::vector<Token>& ts)
{
	int line_n = 3, l_line_n = 1;
	std::unordered_map<std::string, int> g_map;
	std::unordered_map<std::string, int> l_map;
	int if_stage = 0;
	int deep = 0;
	bool def_var = false;
	for (int i = 0; i < ts.size(); ++i)
	{
		auto& s = ts;
		auto& curr = s[i];


		if (if_stage == 0 && curr.body == "function")
			if_stage = 1;

		if (curr.per == '(' && if_stage == 1)
		{
			l_line_n = line_n + 1;
			while (s[i].back != ')')
			{
				if (s[i].body != "" && s[i].back == ':')
				{
					l_map[s[i].body] = l_line_n;
					s[i].body = get_under_line(l_line_n);
					++l_line_n;
				}
				++i;
			}
			if_stage = 2;
		}

		if (curr.per == '{' && if_stage == 2)
		{
			if_stage = 3;
		}
		if (curr.per == '{' && if_stage == 3)
		{
			++deep;
		}

		if (curr.back == '}' && if_stage == 3)
		{
			--deep;
			if (deep == 0)
			{
				if_stage = 0;
				l_map.clear();
				l_line_n = 1;
			}
		}

		if (	(curr.body == "var" && s[i - 1].body == "private")
			|| (curr.body == "var" && s[i - 1].body == "static" && s[i - 2].body == "private") 
			)
		{
			if (if_stage == 0)
			{
				g_map[s[i + 1].body] = line_n;
				++line_n;
			}
			def_var = true;
			continue;
		}

		if (if_stage > 0 && curr.body == "var")
		{
			l_map[s[i + 1].body] = l_line_n;
			++l_line_n;
			
			def_var = true;
			continue;
		}

		if (def_var)
		{
			if (curr.per == ',' && s[i].body != "")
			{
				l_map[s[i].body] = l_line_n;
				++l_line_n;

			}else
			if (curr.back == ',' && s[i + 1].body != "")
			{
				l_map[s[i + 1].body] = l_line_n;
				++l_line_n;
			}
			def_var =  !( curr.back == '\n' || curr.back == ';');
		}

		for (auto it = g_map.begin(); it != g_map.end(); ++it)
		{
			if (it->first == curr.body && curr.per != '"' && curr.back != '"')
			{
				if (s[i - 1].back == '.')
				{
					if (s[i - 1].body == "this")
					{
						curr.body = get_under_line(it->second);
					}
				}
				else
				{
					curr.body = get_under_line(it->second);
				}
			}
		}

		if (if_stage == 3)
		{
			auto& curr_local_map = l_map;
			for (auto it = curr_local_map.begin(); it != curr_local_map.end(); ++it)
			{
				if (it->first == curr.body && curr.per != '"' && curr.back != '"')
				{
					if (s[i - 1].back == '.')
					{
						if (s[i - 1].body == "this")
						{
							curr.body = get_under_line(it->second);
						}
					}
					else
					{
						curr.body = get_under_line(it->second);
					}
				}
			}
		}
	}
}