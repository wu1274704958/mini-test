#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <stack>

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

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string get_parent(Path&& path)
{
    if(path.empty()) return path;
    for(size_t i = path.size() - 1;i >= 0; --i)
    {
        if(path[i] == '/')
        {
            return path.substr(0,i);
        }
    }
    return path;
}

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string to_absolute(Path&& path)
{
    if(path.empty()) return path;
    int b = 0;
    std::stack<std::string> sta;
    for(int i = 0;i < path.size();++i)
    {
        if(path[i] == '/' && b < i)
        {
            auto str = path.substr(b,i - b);
            if(str == ".")
            {}else 
            if(str == "..")
            {
                if(sta.empty())
                    throw std::runtime_error("Bad path for func to_absolute!!");
                sta.pop();
            }else{
                str += '/';
                sta.push(std::move(str));
            }
            b = i + 1;++i;
        }
    }
    if(b < path.size() - 1)
    {
        auto str = path.substr(b,path.size() - b);
        sta.push(std::move(str));
    }
    std::string res;
    while (!sta.empty())
    {
        res.insert(0,sta.top().c_str());
        sta.pop();
    }
    return res;
}

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string get_suffix(Path&& path)
{
    if(path.empty()) return path;
    for(int i = path.size() - 1;i >= 0;--i)
    {
        if(path[i] == '.')
        {
            return path.substr(i + 1,path.size() - i - 1);
        }
    }
    return "";
}

}