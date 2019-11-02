#include "dbg.hpp"
#include "token_stream.hpp"
#include <filesystem>
using namespace token;
namespace fs = std::filesystem;

bool get_rm_path(const char *path, std::string& out);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cout << "No input path!" << std::endl;
		return -1;
	}
	std::string rm_path;
	const char *smail = "\\smali\\";
	if (get_rm_path(argv[1], rm_path))
	{
		for (auto &c : rm_path)
		{
			if (c == '.') c = '\\';
		}
		rm_path.insert(0, smail);
		rm_path.insert(0, argv[1]);
		dbg(rm_path);
		fs::path rm_(rm_path);
		if (fs::exists(rm_))
		{
			fs::remove_all(rm_);
		}
	}
	else {
		std::cout << "Error path!" << std::endl;
		return -1;
	}

	return 0;
}

bool get_rm_path(const char *path,std::string& out)
{
	fs::path p(path);
	fs::path xml = p.append("AndroidManifest.xml");
	dbg(xml);
	if (fs::exists(xml))
	{
		std::ifstream ifs(xml.generic_string(), std::ios::binary);
		TokenStream<std::ifstream> s(std::move(ifs));
		s.analyse();
		auto& ts = s.tokens;

		bool in_manifest = false;
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
					out = ts[i + 1].body;
					dbg(out);
					return true;
				}
			}
		}

	}else
		return false;
}