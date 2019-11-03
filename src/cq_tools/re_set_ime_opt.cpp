#include "token_stream.hpp"
#include <filesystem>
#include "dbg.hpp"
using namespace token;
namespace fs = std::filesystem;

void set_ime_opt(const char* opt,std::vector<Token>& ts);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "No input file!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\wushu\\Desktop\\AndroidStageText.smali");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		/*for (auto& t : ts.tokens)
		{
			std::cout << t << std::endl;
		}*/
		set_ime_opt(argv[2],ts.tokens);
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

void set_ime_opt(const char* opt, std::vector<Token>& ts)
{
	bool in_ctor = false;
	std::string* var_name_p = nullptr;
	int idx = -1;

	for (int i = 0; i < ts.size(); ++i)
	{
		auto& it = ts[i];
		if (!in_ctor && it.per == '.' && it.body == "method" && ts[i + 1].body == "public" && ts[i + 2].body == "constructor")
		{
			in_ctor = true;
		}

		if (in_ctor && it.body == "setImeOptions" && ts[i - 1].per == '-' && ts[i - 1].back == '>')
		{
			int b = i - 1;
			while (ts[b].per != '{')
			{
				--b;
			}
			var_name_p = &(ts[b + 1].body);
			idx = b;
			break;
		}
	}

	dbg(idx);

	if (var_name_p)
	{	
		dbg(*var_name_p);
		while (ts[idx].body != *var_name_p)
		{
			--idx;
		}
		do {
			++idx;
		} while (ts[idx].body.empty());
		ts[idx].body = opt;
	}
}

