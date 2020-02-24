#pragma once

#include <filesystem>
#include <fstream>
#include <optional>

namespace wws{

template<size_t Buf_size,typename Path,typename ...Args>
std::optional<std::string> read_from_file(Path&& path,Args&& ...args)
{
    std::ifstream ifs;
    if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::string>)
    {
        ifs = std::ifstream(path.c_str(),std::forward<Args>(args)...);
    }else if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::filesystem::path>)
    {
        std::string temp =  path.generic_string();
        ifs = std::ifstream(temp.c_str(),std::forward<Args>(args)...);
    }
    if(ifs.good())
    {
        std::string res;
        res.reserve(Buf_size);
		while (!ifs.eof())
		{
			char buf[Buf_size] = { 0 };
			ifs.read(buf, Buf_size - 1);
			res.append(buf);
		}
		return std::optional<std::string>(res);
    }
    else
        return {};
}

template<typename Str,typename Path,typename ...Args>
bool write_to_file(Path&& path,Str&& str,Args&& ...args)
{
    static_assert( std::is_same_v<std::remove_cv_t<std::remove_reference_t<Str>>,std::string>,"this Str must be std::string!");
    std::ofstream ofs;
    if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::string>)
    {
        ofs = std::ofstream(path.c_str(),std::forward<Args>(args)...);
    }else if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::filesystem::path>)
    {
        std::string temp =  path.generic_string();
        ofs = std::ofstream(temp.c_str(),std::forward<Args>(args)...);
    }
    if(ofs.good())
    {
        ofs << str;
		return true;
    }
    else
        return false;
}

}