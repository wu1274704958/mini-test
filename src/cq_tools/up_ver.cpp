#include "token_stream.hpp"
#include "dbg.hpp"
#include <filesystem>
#include <chrono>
#include <sstream>
using namespace token;
namespace fs = std::filesystem;

void up_ver(std::vector<Token>& ts,const char* vn);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "No input path!" << std::endl;
		return -1;
	}

	fs::path f(argv[1]);
	//fs::path f("C:\\Users\\admin\\Desktop\\temp_out_apk\\out_pkg\\AndroidManifest.xml");
	if (fs::exists(f))
	{
		std::ifstream is(f.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> ts(std::move(is));
		ts.analyse();
		
		up_ver(ts.tokens,argv[2]);

		auto out = f.generic_string();
		ts.save(out,true);
	}
	else
	{
		std::cout << "No find input file!" << std::endl;
		return -2;
	}

	return 0;
}

void up_ver(std::vector<Token>& ts,const char* vn)
{
	bool in_mainfest = false;
	int insert = -1;
	for(int i = 0;i < ts.size();++i)
	{
		auto &it = ts[i];
		if(it.body == "manifest" && it.per == '<')
			in_mainfest = true;
		if(in_mainfest)
		{
			while (i < ts.size() && ts[i].back != '>'){++i;}
			if(i < ts.size())
			{
				insert = i;
				ts[i].back = Token::None;
			}
			break;
		}
	}
	dbg(insert);
	if(insert >= 0)
	{
		std::ostringstream ob;

		using namespace std::chrono;
		auto now = system_clock::now();
		auto tt = system_clock::to_time_t(now);
		ob << "android:versionCode=\"" << std::put_time(std::localtime(&tt),"%y%m%d%H%M") << '"'
		<< " android:versionName=\"" << vn << '"';

		Token t(ob.str(),' ','>');
		dbg(t);
		ts.insert(ts.begin() + insert,std::move(t));
	}

}